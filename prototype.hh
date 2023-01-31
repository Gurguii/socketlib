#ifndef GSOCKETS
#define GSOCKETS
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <ifaddrs.h>
#include <netdb.h>
#include <net/if.h>
#include <errno.h>
#include <memory>
#include <thread>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <vector>
#include <variant>
#include <mutex>

#define _XOPEN_SOURCE_EXTENDED 1
#define __TIMEOUT_MULTIPLIER 1000
#define __thread_sleep_ms(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))
#define __thread_sleep_s(x) std::this_thread::sleep_for(std::chrono::seconds(x))

constexpr int BUFF_SIZE = 1024; // read(), recv() default buffer size when reading

struct addressInfo{
    private:
    addrinfo* const head;
    ~addressInfo(){
        // free addrinfo memory when object goes out of scope
        printf("addrinfo running out of scope\n"),
        freeaddrinfo(head);
    }
    public:
    addressInfo(addrinfo *addrs)
    :head(addrs)
    {
        // constructor
    }
    const addrinfo* get(){
        return head;
    };
};

enum class Domain : uint8_t{
    local = AF_LOCAL, // AF_LOCAL == AF_UNIX
    #define local Domain::local
    inet = AF_INET, // tcp/ip protocol - ipv4 addr
    #define inet Domain::inet
    inet6 = AF_INET6 // tcp/ip protocol - ipv6 addr 
    #define inet6 Domain::inet6
};

// defines possible sock types
// when calling gsocket::getsocketpair()
enum class Type : uint8_t{  
    any = 0,
    #define ANY_TYPE Type::any
    tcp = 1, // SOCK_STREAM
    #define stream Type::tcp
    udp = 2 // SOCK_DGRAM
    #define dgram Type::udp
    
};

enum class Behaviour : uint8_t
{
    noblock = 0, // NONBLOCKING FILE DESCRIPTOR - every operation will by default be nonblocking
    #define NOBLOCK Behaviour::noblock
    block = 1 // @default BLOCKING FILE DESCRIPTOR - every operation will by default be blocking
    #define BLOCK Behaviour::block
};

struct msgFrom{
    std::string host;
    int port;
    std::string msg;
};

struct s_preferences{
    Domain family;   // inet(AF_INET) / inet6(AF_INET6)
    Type type;       // stream(SOCK_STREAM) / dgram(SOCK_DGRAM)
    int protocol;    // 
    int flags;       // 
};

namespace gsocket{


    using str = std::string;
    using str_view = std::string_view;

    class CustomExceptions : public std::exception{
        private:
        std::string message;

        public:
        CustomExceptions(std::string msg)
        :message(msg)
        {}

        std::string what(){
            return message;
        }
    };
    int availableBytes(int __sock){
        int avBytes;
        ioctl(__sock, FIONREAD, &avBytes);
        return avBytes;
    }
    // Returns Domain(Ipv4|Ipv6) addr of interface E.g eth0, lo, docker0
    str getIpByIface(str_view ifa, Domain &&t = inet)
    {
        ifaddrs *addrs = nullptr;
        getifaddrs(&addrs);
        str iface((t == inet ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN), '\x00');
        for(auto a = addrs ; a!=nullptr; a = a->ifa_next)
        {
            if(a->ifa_addr->sa_family == static_cast<int>(t))
            {
                if(strcmp(ifa.data(), a->ifa_name)){
                    continue;
                }
                if(static_cast<int>(t) == AF_INET){
                    inet_ntop(a->ifa_addr->sa_family, &((reinterpret_cast<sockaddr_in*>(a->ifa_addr))->sin_addr), &iface[0], INET_ADDRSTRLEN);
                }else if(static_cast<int>(t) == AF_INET6){
                    inet_ntop(a->ifa_addr->sa_family, &((reinterpret_cast<sockaddr_in6*>(a->ifa_addr))->sin6_addr), &iface[0], INET6_ADDRSTRLEN);
                }
                break;
            }
        }
        freeifaddrs(addrs);
        return iface;
    }
    // Return addrinfo pointer which can be used to connect/bind to
    // @param host E.g www.google.com
    // @param service/port E.g 443 == https 
    // @param hints This is a s_preferences struct which holds data refered to a prefered socket. 
    // e.g. s_preferences{INET, stream, 0, 0} would tell the function that you prefer a stream/inet socket
    addressInfo getaddrinfo(str_view host, str_view service, s_preferences *hints)
    {
        addrinfo *addrs = nullptr;
        addrinfo pref{
            // required for hint
            .ai_flags = static_cast<int>(hints->flags),
            .ai_family = static_cast<int>(hints->family), 
            .ai_socktype = static_cast<int>(hints->type),
            .ai_protocol = static_cast<int>(hints->protocol),
            // not required
            .ai_addrlen = 0,
            .ai_addr = nullptr,
            .ai_canonname = nullptr,
            .ai_next = nullptr
        };

        if(int&& a = ::getaddrinfo(&host[0], &service[0], &pref, &addrs))
        {
            fprintf(stderr, "%s\n", gai_strerror(a));
            return nullptr;
            //throw CustomExceptions("getaddrinfo failed\n");
        }
        if(addrs->ai_next != nullptr){
            throw CustomExceptions("error - this shouldn't have happened, getaddrinfo() returned more than 1 socket using hints\n");
        }
        return addressInfo(addrs);
    }
    std::pair<str, str> getnameinfo(sockaddr* addr, socklen_t addrlen)
    {
        str _host(46, '\x00'), _service(46, '\x00');
        if(::getnameinfo(addr, addrlen, &_host[0], 46, &_service[0], 46, 0))
        {
            throw CustomExceptions("getnameinfo failed\n");
        }
        return {_host,_service};
    }
    std::pair<str, str> getnameinfo(addressInfo* addr)
    {
        str _host(46, '\x00'), _service(46, '\x00');
        if(::getnameinfo(addr->get()->ai_addr, addr->get()->ai_addrlen, &_host[0], 46, &_service[0], 46, 0))
        {
            throw CustomExceptions("getnameinfo failed\n");
        }
        return {_host,_service};
    }
    class __sw // POSIX socket methods wrapper
    {
        private:
        int sock, domain, type, protocol;

        protected:
        __sw(int d, int t, int p)
        :domain(d), type(t), protocol(p), sock(::socket(d, t, p))
        {
            // main constructor
        }
        __sw(int __sock)
        :sock(__sock)
        {
            // socket wrapper
        }
        ~__sw(){
            // just in case
            ::close(sock);
        }
        
        public:
        int getSock(){
            return sock;
        }
        void close()
        {
            ::close(sock);
        }
        // idk if this is worth, the idea is to avoid destructing and constructing
        // to make a new socket of same characteristics
        void reset()
        {
            ::close(sock);
            sock = ::socket(domain, type, protocol);
        }
        // Returns std::pair containing socket's ip and port
        std::pair<std::string, int> getsockname()
        { 
            if(domain == AF_INET){
                sockaddr_in addr;
                socklen_t addrlen = sizeof(addr);
                ::getsockname(sock, reinterpret_cast<sockaddr*>(&addr), &addrlen);
                return {inet_ntoa(addr.sin_addr), htons(addr.sin_port)};
            }else if(domain == AF_INET6){
                sockaddr_in6 addr;
                socklen_t addrlen = sizeof(addr);
                ::getsockname(sock, reinterpret_cast<sockaddr*>(&addr), &addrlen);
                // isn't this bad if the f gets called often?
                std::string ad(46, '\x00');
                inet_ntop(AF_INET6, &addr.sin6_addr, &ad[0], INET6_ADDRSTRLEN);
                return {ad, htons(addr.sin6_port)};
            }
            return {nullptr, 0};
        }
        // Returns std::pair containing socket peer's ip and port
        std::pair<std::string, int> getpeername()
        {
            if(domain == AF_INET){
                sockaddr_in addr;
                socklen_t addrlen = sizeof(addr);
                ::getpeername(sock, reinterpret_cast<sockaddr*>(&addr), &addrlen);
                return {inet_ntoa(addr.sin_addr), htons(addr.sin_port)};
            }else if(domain == AF_INET6){
                sockaddr_in6 addr;
                socklen_t addrlen = sizeof(addr);
                ::getpeername(sock, reinterpret_cast<sockaddr*>(&addr), &addrlen);
                std::string ad(46, '\x00');
                inet_ntop(AF_INET6, &addr.sin6_addr, &ad[0], INET6_ADDRSTRLEN);
                return {ad, htons(addr.sin6_port)};
            }
            return {nullptr, 0};
        }
        int accept()
        {
            int s;
            if(domain == AF_INET){
                sockaddr_in addr;
                socklen_t addrlen = sizeof(addr);
                s = ::accept(sock, (sockaddr *)&addr, &addrlen);       
            }else if(domain == AF_INET6){
                sockaddr_in6 addr;
                socklen_t addrlen = sizeof(addr);
                s = ::accept(sock, (sockaddr *)&addr, &addrlen);
            }
            return s;
        }
        // Bind to port ready to listen in any address
        int bind(int port, const char *iface = nullptr)
        {
            if(domain != AF_INET && domain != AF_INET6){
                throw CustomExceptions("domain must be inet / inet6\n");
            }
            if(domain == AF_INET){
                sockaddr_in addr;
                int opt = 1;
                if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
                {
                    throw CustomExceptions("set socket failed " + std::string(strerror(errno)));
                }
                addr.sin_family = domain;
                addr.sin_port = htons(port);
                addr.sin_addr.s_addr = INADDR_ANY;
                if(::bind(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
                {
                    throw CustomExceptions("binding failed " + std::string(strerror(errno)));
                }
            }
            else if(domain == AF_INET6){
                if(iface == nullptr){
                    throw CustomExceptions("iface can't be null with Ipv6\n");
                }
                sockaddr_in6 addr;
                int opt = 1;
                if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
                {
                    throw CustomExceptions("can't set socket" + std::string(strerror(errno)));
                }
                addr.sin6_family = AF_INET6;    
                addr.sin6_port = htons(port);
                if(!(addr.sin6_scope_id = if_nametoindex(iface))){
                    fprintf(stderr, "nametoindex failed\n");
                };
                if (inet_pton(AF_INET6, iface, &addr.sin6_addr) < 0)
                { 
                    throw CustomExceptions("Invalid / unsupported inet6 address");
                };
                if(::bind(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
                {
                    throw CustomExceptions("Can't bind to port");
                }   
            }
            return 0;
        }

        // Binds socket to
        int bind(str_view iface, int port)
        {
            if(domain != AF_INET && domain != AF_INET6){
                throw CustomExceptions("domain must be inet / inet6\n");
            }
            str inet_iface(46, '\x00');
            inet_iface = iface;
            if(iface.empty())
            {
                inet_iface = (domain == AF_INET ? "0.0.0.0" : "::1"); // all local ipv4/ipv6 addresses
            } 
            if(domain == AF_INET)
            {
                sockaddr_in addr;
                int opt = 1;

                if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
                {
                    throw CustomExceptions("set socket failed " + std::string(strerror(errno)));
                }

                addr.sin_family = domain;
                addr.sin_port = htons(port);

                if (inet_pton(domain, &inet_iface[0], &addr.sin_addr.s_addr) < 0)
                { 
                    throw CustomExceptions("invalid / unsupported inet address " + std::string(strerror(errno)));
                };

                if(::bind(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
                {
                    throw CustomExceptions("can't bind to port " + std::string(strerror(errno)));
                }
            }else if(domain == AF_INET6)
            {
                sockaddr_in6 addr;
                int opt = 1;

                if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
                {
                    throw CustomExceptions("can't set socket" + std::string(strerror(errno)));
                }

                addr.sin6_family = AF_INET6;    
                addr.sin6_port = htons(port);
                //if(!(addr.sin6_scope_id = if_nametoindex("eth0"))){
                //    fprintf(stderr, "nametoindex failed\n");
                //};
                printf("trying to put ip => %s\n", inet_iface.c_str());
                if (inet_pton(AF_INET6, &inet_iface[0], &addr.sin6_addr) < 0)
                { 
                    throw CustomExceptions("Invalid / unsupported inet6 address");
                };

                if(::bind(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
                {
                    throw CustomExceptions("Can't bind to port");
                }   
            }
            return 0;  
        }
        /*
        Start listening so connections can be accepted
        __maxconns: max amount of connections to queue before refusing
        */
        int listen(int maxconns = 3)
        {
            return ::listen(sock, maxconns);
        }
        /*  
            Connection-oriented sockets (stream) it tries to connect to host:port
            Connectionless sockets (dgram) it sets default host:port for further send() calls(faster than constantly calling sendto())
            Returns 0 on sucess 1 on failure
            @param host ip, gsocket::getIpByIface() can be used to retrieve addresses
            @param port port
            @param iface target iface, e.g. eth0. Required JUST when calling connect with Ipv6 sockets
        */
        int connect(str_view host, int port, const char *iface = nullptr)
        {
            if(domain == AF_INET){
                sockaddr_in addr;
                addr.sin_family = AF_INET;
                addr.sin_port = htons(port);
                // Check address and assign it to host struct
                if (inet_pton(domain, &host[0], &addr.sin_addr) <= 0){
                    return 1;
                }
                if(::connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)))
                {
                    return 1;
                }
            }else if(domain == AF_INET6){
                if(iface == nullptr){
                    throw CustomExceptions("iface can't be null when using ipv6 addresses\n");
                }
                sockaddr_in6 addr;
                addr.sin6_family = AF_INET6;
                addr.sin6_port = htons(port);
                addr.sin6_scope_id = if_nametoindex(iface);
                if(inet_pton(domain, &host[0], &addr.sin6_addr) <= 0){
                    return 1;
                }
                if(::connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)))
                {
                    return 1;
                }
            }
            return 0;
        }
        // Attempts connecting to target, addrinfo* struct can be retrieved from getaddrinfo()
        int connect(const addrinfo *target){
            return ::connect(sock, target->ai_addr, target->ai_addrlen);
        }

        int connect(addressInfo &target){
            return ::connect(sock, target.get()->ai_addr, target.get()->ai_addrlen);
        }
        // Sends data through socket
        // returns bytes sent
        int send(str_view data)
        {
            return ::send(sock, &data[0], data.size(), 0);
        }
        // Sends N bytes of data through socket
        // returns bytes sent
        int send(str_view data, int N)
        {
            return ::send(sock, &data[0], N, 0);
        }
        // Read data from socket
        str read()
        {
            str data;
            str buffer(BUFF_SIZE, '\x00');
            int last;
            for(;;)
            {
                if((last = ::read(sock, &buffer[0], BUFF_SIZE)) < BUFF_SIZE)
                {
                    buffer.resize(last);
                    data+=buffer;
                    break;
                }
                data+=buffer;
            }
            return data;
        }
        // Read N bytes of data from socket
        str read(int N)
        {
            str buffer(N, '\x00');
            ::read(sock, &buffer[0], N);
            return buffer;
        }  
        // Awaits til data is received, reads every byte available into buffer
        // returns 0 if connection closed, -1 for errors, -2 for timeout else bytes read
        // @param buffer pointer to string that will store the data 
        // @param timeout seconds, by default blocks until data is available
        int awaitData(std::string *buffer, int timeout = -1){
            auto _s_poll = pollfd{
                .fd = sock,
                .events = POLLIN
            };
            int event, avBytes, rStatus;
            event = poll(&_s_poll, 1, (timeout == -1 ? -1 : (timeout * __TIMEOUT_MULTIPLIER)));
            if(event > 0 && (_s_poll.revents & POLLIN)){
                // data available
                ioctl(sock, FIONREAD, &avBytes);
                if(buffer->size() < avBytes){
                    buffer->resize(avBytes);
                }
                return ::recv(sock, buffer->data(), avBytes, 0);
            }
            // timeout
            return -2;
        }

        int awaitDataFrom(msgFrom *__sockHostData, int timeout = -1){
            auto _s_poll = pollfd{
                .fd = sock,
                .events = POLLIN
            };
            int event, avBytes, rStatus;
            event = poll(&_s_poll, 1, (timeout == -1 ? -1 : (timeout * __TIMEOUT_MULTIPLIER)));
            if(event > 0 && (_s_poll.revents & POLLIN)){
                // data available
                ioctl(sock, FIONREAD, &avBytes);
                if(__sockHostData->msg.size() < avBytes){
                    __sockHostData->msg.resize(avBytes);
                }
                sockaddr addr;
                socklen_t &&addrlen = sizeof(addr);
                int &&n = ::recvfrom(sock, __sockHostData->msg.data(), avBytes, 0, &addr, &addrlen);
                if(n != -1){
                    __sockHostData->host = inet_ntoa(reinterpret_cast<sockaddr_in*>(&addr)->sin_addr);
                    __sockHostData->port = htons(reinterpret_cast<sockaddr_in*>(&addr)->sin_port);
                }
                return n;
                //return ::recv(sock, __sockHostData->msg.data(), avBytes, 0);
            }
            // timeout
            return -2;
        }
        // Returns all data from socket (blocks until data available or error)
        str recv()
        {
            str data;
            str buffer(BUFF_SIZE, '\x00');
            int last = 0;
            for(;;)
            {
                last = ::recv(sock, &buffer[0], BUFF_SIZE, 0);
                if(last == 0){
                    // client closed connection
                    return data;
                }
                if (last == -1) {
                    throw gsocket::CustomExceptions("recv failed: " + std::string(strerror(errno)));
                }
                if(last < BUFF_SIZE)
                {
                    buffer.resize(last);
                    data+=buffer;
                    break;
                }
                data+=buffer;
            }
            return data;
        }
        // Reads N bytes of data from socket
        str recv(int N)
        {
            str data(N, '\x00');
            ::recv(sock, &data[0], N, 0);
            return data;
        }
        // Send 'data' to 'host' on 'port', returns bytes sent
        int sendto(str_view host, int port, str_view data)
        {
            int n;
            if(domain == AF_INET){
                sockaddr_in t;
                t.sin_family = domain;
                t.sin_port = htons(port);
                inet_pton(AF_INET, &host[0], &t.sin_addr);
                n = ::sendto(sock, &data[0], data.size(), 0, reinterpret_cast<sockaddr*>(&t), sizeof(t));
            }else if(domain == AF_INET6){
                sockaddr_in6 t;
                t.sin6_family = domain;
                t.sin6_port = htons(port);
                inet_pton(AF_INET6, &host[0], &t.sin6_addr);
                n = ::sendto(sock, &data[0], data.size(), 0, reinterpret_cast<sockaddr*>(&t), sizeof(t));
            }
            return n;
        }
        /* 
            Receive N bytes of data (default 1024)
            returns msgFrom struct with info about peer socket and
            data received
            struct msgFrom{
                const char *host;
                int port;
                const char *msg;
            }
            @param N amount of data to receive
        */
        msgFrom recvfrom(int N = BUFF_SIZE)
        {
            if(domain == AF_INET){
                sockaddr_in addr;
                socklen_t addrlen = sizeof(addr);
                str data(N, '\x00');
                ::recvfrom(sock, &data[0], N, 0, reinterpret_cast<sockaddr*>(&addr), &addrlen);
                return msgFrom{inet_ntoa(addr.sin_addr), htons(addr.sin_port), data};
            }else if(domain == AF_INET6){
                sockaddr_in6 addr;
                socklen_t addrlen = sizeof(addr);
                str data(N, '\x00');
                ::recvfrom(sock, &data[0], N, 0, reinterpret_cast<sockaddr*>(&addr), &addrlen);
                str ad(46, '\x00');
                inet_ntop(AF_INET6, &addr.sin6_addr, &ad[0], INET6_ADDRSTRLEN);
                return msgFrom{ad, htons(addr.sin6_port), data};
            }
            return msgFrom{nullptr, 0, 0};
        }
    };

    class socket : public __sw
    {
        public:
        // socket wrapper
        socket(int s)
        :__sw(s)
        {
        }
        // Raw socket constructor, it takes POSIX socket() arguments
        // socket() - https://man7.org/linux/man-pages/man2/socket.2.html
        socket(int domain, int type, int protocol)
        :__sw(domain, type, protocol)
        {}
        // NOTE -> gsocket::socket(AF_INET, SOCK_STREAM, 0) == SAME AS == gsocket::socket(INET, stream, BLOCK);
        socket(Domain domain, Type type, Behaviour b)
        :__sw(static_cast<int>(domain), (b == BLOCK ? static_cast<int>(type) : (static_cast<int>(type) | SOCK_NONBLOCK)), 0)
        {
        }
    };
    class tcp_socket : public __sw
    {
        public:
        tcp_socket(Domain f = inet, Behaviour b = BLOCK)
        :__sw(static_cast<int>(f), (static_cast<int>(b) ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)), 0)
        {
            // default constructor
        }
        gsocket::socket accept_connection()
        {
            return __sw::accept();
        }
    };
    
    class tcp_client : public __sw
    {
        private:
        int __status = 0;

        public:    
        tcp_client(str_view host, int port, Domain f = inet, Behaviour p = BLOCK, const char *iface = nullptr)
        :__sw(static_cast<int>(f), (static_cast<int>(p) ? (SOCK_STREAM) : (SOCK_STREAM | SOCK_NONBLOCK)), 0)
        {
            __status = __sw::connect(host, port, iface);
        }

        tcp_client(const addrinfo *addr, Domain f = inet, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), (static_cast<int>(p) ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)), 0)
        {
            __status = __sw::connect(addr);
        }

        int failure()
        {
            return __status;
        }
    };

    class tcp_server : public __sw
    {
        public:
        tcp_server(str_view addr_iface, int port, Domain f = inet, int maxconns = 3, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), static_cast<int>(p) ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK), 0)
        {
            __sw::bind(addr_iface, port);
            __sw::listen(maxconns);
        }

        tcp_server(int port, Domain f = inet, int maxconns = 3, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), (static_cast<int>(p) ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)), 0){
            __sw::bind(port);
            __sw::listen(maxconns);
        }

        gsocket::socket accept_connection(){
            return gsocket::socket(__sw::accept());
        }
    };

    class udp_socket : public __sw{
        public:
        // Behaviour = BLOCK | NOBLOCK
        // defines file descriptor default behaviour
        udp_socket(Domain f = inet, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), static_cast<int>(p) ? SOCK_DGRAM : (SOCK_DGRAM | SOCK_NONBLOCK), 0)
        {}
    };

    class udp_client : public __sw{
        public:
        // set default host port for future send() calls
        udp_client(str_view host, int port, Domain f = inet, Behaviour p = BLOCK, const char *iface = nullptr)
        :__sw(static_cast<int>(f), static_cast<int>(p) ? SOCK_DGRAM : (SOCK_DGRAM | SOCK_NONBLOCK), 0)
        {
            __sw::connect(host, port, iface);
        }
    };

    class udp_server : public __sw
    {
        public:
        /*
            bind to port and listen in given address/iface
            if addr_iface is empty, 0.0.0.0 will be chosen,
            meaning it will receive data from any local ipv4 address
        */
        udp_server(str_view addr_iface, int port, Domain f = inet, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), static_cast<int>(p) ? SOCK_DGRAM : (SOCK_DGRAM | SOCK_NONBLOCK), 0)
        {
            __sw::bind(addr_iface, port);   
        }

        // bind to port and listen in any local ipv4 address
        udp_server(int port, Domain f = inet, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), static_cast<int>(p) ? SOCK_DGRAM : (SOCK_DGRAM | SOCK_NONBLOCK), 0)
        {
            __sw::bind(port); 
        }
    };

    /* 
        Returns 2 AF_LOCAL sockets (tcp/udp) wrapped with gsocket::socket() class 
        socketpair (CHECK NOTES) - https://man7.org/linux/man-pages/man2/socketpair.2.html
        socket domains & types - https://man7.org/linux/man-pages/man2/socket.2.html
    */
    std::pair<gsocket::socket, gsocket::socket> getsocketpair(Type type, Behaviour b = BLOCK)
    {   
        std::pair<int, int>fds;
        if(::socketpair(AF_LOCAL, (static_cast<int>(b) ? static_cast<int>(type) : (static_cast<int>(type) | SOCK_NONBLOCK)), 0, &fds.first))
        {
            throw CustomExceptions("socketpair error - "+std::string(strerror(errno)));
        }
        return {gsocket::socket(fds.first), gsocket::socket(fds.second)}; 
    }
}
#endif