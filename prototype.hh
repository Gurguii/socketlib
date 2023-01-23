#ifndef GSOCKETS
#define GSOCKETS

#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <ifaddrs.h>
#include <netdb.h>
#include <net/if.h>
#include <errno.h>
#include <iostream> // TEMP
#include <memory>

#define _XOPEN_SOURCE_EXTENDED 1

constexpr int BUFF_SIZE = 1024; // read(), recv() default buffer size when reading

/*
    Some recv() flags:
    
    MSG_WAITFORONE - wait for at least one packet to return
*/
class addressInfo
{
    private:
    std::unique_ptr<addrinfo> head;
    void freeme(){
        freeaddrinfo(head.get());
    }
    
    public:
    addressInfo(addrinfo *addrs)
    :head(std::make_unique<addrinfo>(*addrs))
    {
        // constructor
    }
    std::pair<sockaddr *, socklen_t> get_connect(){
        return{head.get()->ai_addr, head.get()->ai_addrlen};
    };

    ~addressInfo(){
        printf("destructor\n");
    }
};

enum class Domain : uint8_t
{
    local = AF_LOCAL, // AF_LOCAL == AF_UNIX
    #define local Domain::local
    inet = AF_INET, // tcp/ip protocol - ipv4 addr
    #define inet Domain::inet
    inet6 = AF_INET6 // tcp/ip protocol - ipv6 addr 
    #define inet6 Domain::inet6
};

// defines possible sock types
// when calling gsocket::getsocketpair()
enum class Type : uint8_t
{   
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

struct sock_data_
{
    const char *host;
    int port;
    std::string msg;
};

struct s_preferences
{
    Domain family;   // INET or INET6
    Type type;       // stream or dgram
    int protocol;    // 
    int flags;       // 
};

namespace gsocket
{
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
    // Returns ipv4 addr of interface E.g eth0, lo, docker0
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
            .ai_flags = hints->flags,
            .ai_family = static_cast<int>(hints->family), 
            .ai_socktype = static_cast<int>(hints->type),
            .ai_protocol = hints->protocol,
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
        //&((sockaddr_in6*)(addrs->ai_addr))->sin6_addr)
        //&(static_cast<sockaddr_in6*>(addrs->ai_addr)->sin6_addr)
        if(addrs->ai_next == nullptr){
            printf("NULLPTR\n");
        }
        return addressInfo(addrs);
    }
    std::pair<str, str> getnameinfo(sockaddr* addr, socklen_t addrlen)
    {
        str h(46, '\x00');
        str s(46, '\x00');
        if(::getnameinfo(addr, addrlen, &h[0], 46, &s[0], 46, 0))
        {
            throw CustomExceptions("getnameinfo failed\n");
        }
        return {h,s};
    }
    class __sw // POSIX socket methods wrapper
    {
        protected:
        __sw(int d, int t, int p)
        {
            // main constructor
            domain = d;
            type = t;
            protocol = p;
            sock = ::socket(d, t, p);
        }
        __sw(int __sock)
        {
            // socket wrapper
            sock = __sock;
        }
        ~__sw(){
            ::close(sock);
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
            if(s == -1){
                throw CustomExceptions("accept failed: " + std::string(strerror(errno)));
            }
            return s;
        }
        
        public:
        int sock, domain, type, protocol;

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
        std::pair<char*, int> getsockname()
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
                char *ad;
                inet_ntop(AF_INET6, &addr.sin6_addr, ad, INET6_ADDRSTRLEN);
                return {ad, htons(addr.sin6_port)};
            }
            return {nullptr, 0};
        }
        // Returns std::pair containing socket peer's ip and port
        std::pair<char*, int> getpeername()
        {
            if(domain == AF_INET){
                sockaddr_in addr;
                socklen_t addrlen = sizeof(addr);
                ::getpeername(sock, reinterpret_cast<sockaddr*>(&addr), &addrlen);
                return {inet_ntoa(addr.sin_addr), htons(addr.sin_port)};
            }else if(domain == AF_INET6){
                sockaddr_in6 addr;
                socklen_t addrlen = sizeof(addr);
                
            }

        }

        // Bind to port ready to listen in any address
        int bind(int port)
        {
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
            return 0;
        }

        // Binds socket to
        int bind(str_view iface, int port)
        {
            str inet_iface(46, '\x00');
            inet_iface = iface;
            if(iface.empty())
            {
                inet_iface = "0.0.0.0"; // all local ipv4 addresses
            } 
            if(domain == AF_INET)
            {
                sockaddr_in addr;
                int opt = 1;

                if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
                {
                    throw CustomExceptions("Can't set socket");
                }

                addr.sin_family = domain;
                addr.sin_port = htons(port);

                if (inet_pton(domain, &inet_iface[0], &addr.sin_addr.s_addr) < 0)
                { 
                    throw CustomExceptions("Invalid / unsupported inet address");
                };

                if(::bind(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
                {
                    throw CustomExceptions("Can't bind to port");
                }
            }else if(domain == AF_INET6)
            {
                sockaddr_in6 addr;
                int opt = 1;

                if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
                {
                    throw CustomExceptions("Can't set socket");
                }

                addr.sin6_family = AF_INET6;    
                addr.sin6_port = htons(port);
                if(!(addr.sin6_scope_id = if_nametoindex("eth0"))){
                    fprintf(stderr, "nametoindex failed\n");
                };
                printf("trying to put ip => %s\n", inet_iface.c_str());
                if (inet_pton(AF_INET6, inet_iface.c_str(), &addr.sin6_addr) < 0)
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
        */
        int connect(str_view host, int port)
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
                sockaddr_in6 addr;
                addr.sin6_family = AF_INET6;
                addr.sin6_port = htons(port);
                //addr.sin6_scope_id = if_nametoindex("eth0");
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
        int connect(addrinfo *target)
        {
            return ::connect(sock, target->ai_addr, target->ai_addrlen);
        }

        int connect(sockaddr *addr, socklen_t &addrlen){
            return::connect(sock, addr, addrlen);
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
        // Returns all data from socket
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
            sockaddr_in t;
            t.sin_family = domain;
            t.sin_port = htons(port);
            inet_pton(AF_INET, &host[0], &t.sin_addr);
            int n = ::sendto(sock, &data[0], data.size(), 0, (struct sockaddr *)&t, sizeof(t));
            return n;
        }
        /* 
            Receive N bytes of data (default BUFF_SIZE = 1024) and return rhost, rport, and data received 
            struct sock_data_{
                const char *host;
                int port;
                const char *msg;
            }
            @param N amount of data to receive
        */
        sock_data_ recvfrom(int N = BUFF_SIZE)
        {
            sockaddr_in addr;
            int addrlen = sizeof(addr);
            str data(BUFF_SIZE, '\x00');
            ::recvfrom(sock, &data[0], N, 0, (sockaddr *)&addr, (socklen_t *)&addrlen);
            return sock_data_{inet_ntoa(addr.sin_addr), htons(addr.sin_port), data};
        }
    };

    class socket : public __sw
    {
        public:
        // socket wrapper
        socket(int s)
        :__sw(s)
        {}
        // Raw socket constructor, it takes POSIX socket() arguments
        // socket() - https://man7.org/linux/man-pages/man2/socket.2.html
        socket(int domain, int type, int protocol)
        :__sw(domain, type, protocol)
        {}
        // NOTE -> gsocket::socket(AF_INET, SOCK_STREAM, 0) == SAME AS == gsocket::socket(INET, stream, BLOCK);
        socket(Domain domain, Type type, Behaviour b)
        :__sw(static_cast<int>(domain), (b == BLOCK ? static_cast<int>(type) : (static_cast<int>(type) | SOCK_NONBLOCK)), 0)
        {}
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
        int status = 0;

        public:    
        tcp_client(str_view host, int port, Domain f = inet, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), (static_cast<int>(p) ? (SOCK_STREAM) : (SOCK_STREAM | SOCK_NONBLOCK)), 0)
        {
            status = !__sw::connect(host, port);
        }

        tcp_client(sockaddr* addr, socklen_t addrlen, Domain f = inet, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), (static_cast<int>(p) ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)), 0)
        {
            status = !__sw::connect(addr, addrlen);
        }

        int connected()
        {
            return status;
        }
    };

    class tcp_server : public __sw
    {
        private:
        uint8_t __status = 1;

        public:
        tcp_server(str_view addr_iface, int port, Domain f = inet, int maxconns = 3, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), static_cast<int>(p) ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK), 0)
        {
            if((__sw::bind(addr_iface, port) || __sw::listen(maxconns)))
            {
                __status = 0;
            }; 
        }

        tcp_server(int port, Domain f = inet, int maxconns = 3, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), (static_cast<int>(p) ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)), 0){

            if((__sw::bind(port)) || ((__sw::listen(maxconns))))
            {
                __status = 0;
            };
        }

        gsocket::socket accept_connection(){
            return gsocket::socket(__sw::accept());
        }

        int up(){
            return __status;
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
        udp_client(str_view host, int port, Domain f = inet, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), static_cast<int>(p) ? SOCK_DGRAM : (SOCK_DGRAM | SOCK_NONBLOCK), 0)
        {
            __sw::connect(host, port);
        }
    };

    class udp_server : public __sw
    {
        private:
        uint8_t __status = 0;

        public:
        /*
            bind to port and listen in given address/iface
            if addr_iface is empty, 0.0.0.0 will be chosen,
            meaning it will receive data from any local ipv4 address
        */
        udp_server(str_view addr_iface, int port, Domain f = inet, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), static_cast<int>(p) ? SOCK_DGRAM : (SOCK_DGRAM | SOCK_NONBLOCK), 0)
        {
            __status == !__sw::bind(addr_iface, port);   
        }

        // bind to port and listen in any local ipv4 address
        udp_server(int port, Domain f = inet, Behaviour p = BLOCK)
        :__sw(static_cast<int>(f), static_cast<int>(p) ? SOCK_DGRAM : (SOCK_DGRAM | SOCK_NONBLOCK), 0)
        {
            __status == !__sw::bind(port); 
        }

        int binded()
        {
            return __status;
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
            throw CustomExceptions("Couldn't create socketpair");
        }
        return {gsocket::socket(fds.first), gsocket::socket(fds.second)}; 
    }
}
#endif