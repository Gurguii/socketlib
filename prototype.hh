#ifndef GSOCKETS
#define GSOCKETS

#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <ifaddrs.h>
#include <netdb.h>
#include <errno.h> // NOT IN USE
#include <iostream> // TEMP

#define __tcp_block_sock socket(AF_INET, SOCK_STREAM, 0)
#define __tcp_noblock_sock socket(AF_INET, (SOCK_STREAM | SOCK_NONBLOCK), 0)
#define __udp_block_sock socket(AF_INET, SOCK_DGRAM, 0)
#define __udp_noblock_sock socket(AF_INET, (SOCK_DGRAM | SOCK_NONBLOCK), 0)

#define _XOPEN_SOURCE_EXTENDED 1

constexpr int BUFF_SIZE = 1024; // read(), recv() default buffer size when reading

/*
    Some recv() flags:
    
    MSG_WAITFORONE - wait for at least one packet to return
*/

enum class s_family : uint8_t
{
    local = 1, // AF_LOCAL == AF_UNIX
    #define LOCAL s_family::local
    inet = 2, // tcp/ip protocol - ipv4 addr
    #define INET s_family::inet
    inet6 = 10 // tcp/ip protocol - ipv6 addr 
    #define INET6 s_family::inet6
};

// defines possible sock types
// when calling gsocket::getsocketpair()
enum class s_type : uint8_t
{   
    any = 0,
    #define ANY_TYPE s_type::any
    tcp = 1, // SOCK_STREAM
    #define TCP s_type::tcp
    udp = 2 // SOCK_DGRAM
    #define UDP s_type::udp
    
};

enum class socket_behaviour : uint8_t
{
    noblock = 0, // NONBLOCKING FILE DESCRIPTOR - every operation will by default be nonblocking
    #define NOBLOCK socket_behaviour::noblock
    block = 1 // BLOCKING FILE DESCRIPTOR - every operation will by default be blocking
    #define BLOCK socket_behaviour::block
};

struct sock_data_
{
    const char *host;
    int port;
    std::string msg;
};

struct s_preferences
{
    s_family family; // INET or INET6
    s_type type;     // TCP or UDP
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
    str getIpByIface(str_view ifa, s_family t = INET)
    {
        ifaddrs *addrs = nullptr;
        getifaddrs(&addrs);

        str iface(46, '\x00'); // Ipv6 length so both choices fit

        for(auto a = addrs ; a!=nullptr; a = a->ifa_next)
        {
            if(a->ifa_addr->sa_family == static_cast<int>(t))
            {
                if(!strcmp(ifa.data(), a->ifa_name))
                {
                    inet_ntop(a->ifa_addr->sa_family, &((sockaddr_in*)(a->ifa_addr))->sin_addr, &iface[0], INET_ADDRSTRLEN);
                    return iface;
                }
            }
        }
        return iface;
    }

    // @Return addrinfo pointer which can be used to connect/bind to
    // @param host E.g www.google.com
    // @param service/port E.g 443 == https 
    // @param hints This is a s_preferences struct which holds data refered to a prefered socket. 
    // E.g s_preferences{INET, TCP, 0, 0} would tell the function that you prefer a TCP/Ipv4 socket
    addrinfo* getaddrinfo(str_view host, str_view service = NULL, s_preferences *hints = nullptr)
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

        if(::getaddrinfo(&host[0], &service[0], &pref, &addrs))
        {
            throw CustomExceptions("getaddrinfo failed\n");
        }
        
        str addr(46, '\x00'); // Ipv6 length so both Ipv4 & 6 fit
        inet_ntop(addrs->ai_addr->sa_family, &((sockaddr_in*)(addrs->ai_addr))->sin_addr, &addr[0], INET_ADDRSTRLEN);
        printf("Returning socket %i : %s : %s\n", addrs->ai_socktype, &addr[0], addrs->ai_canonname);
        /* Why is this working? I might be missunderstanding the freeaddrinfo use, shouldn't it empty the whole list? */
        freeaddrinfo(addrs);
        return addrs;
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
        public:
        const char *error;

        protected:

        __sw(int domain, int type, int protocol)
        {
            // main constructor
            this->domain = domain;
            this->type = type;
            this->protocol = protocol;
            this->sock = ::socket(domain, type, protocol);
        }

        __sw(int sock)
        {
            // socket wrapper
            this->sock = sock;
        }

        int accept()
        {
            sockaddr_in newsock;
            int addrlen = sizeof(newsock);
            return ::accept(this->sock, (sockaddr *)&newsock, (socklen_t *)&addrlen);
        }

        public:
        int sock, domain, type, protocol;

        void close()
        {
            ::close(this->sock);
        }
        
        // idk if this is worth, the idea is to avoid destructing and constructing
        // to make a new socket of same characteristics
        void reset()
        {
            ::close(this->sock);
            this->sock = ::socket(this->domain, this->type, this->protocol);
        }

        std::pair<char*, int> getsockname()
        {
            // Returns std::pair containing socket's ip and port 
            sockaddr_in s;
            int slen = sizeof(s);
            ::getsockname(this->sock, (sockaddr*)&s, (socklen_t*)&slen);
            return {inet_ntoa(s.sin_addr), htons(s.sin_port)};
        }
        
        std::pair<char*, int> getpeername()
        {
            // Returns std::pair containing socket peer's ip and port
            sockaddr_in s;
            int addrl = sizeof(s);
            ::getpeername(this->sock, (sockaddr *)&s, (socklen_t *)&addrl);
            return {inet_ntoa(s.sin_addr), htons(s.sin_port)};
        }

        // Bind to port ready to listen in any address
        int bind(int port)
        {
            sockaddr_in addr;
            int opt = 1;

            if(setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
            {
                return true;
            }

            addr.sin_family = this->domain;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = INADDR_ANY;

            if(::bind(this->sock, (sockaddr *)&addr, sizeof(addr)) < 0)
            {
                return 1;
            }
            return 0;
        }

        // Binds socket to 
        int bind(str_view iface, int port)
        {
            str inet_iface{iface};
            // Check if iface is an address or iface name (E.g eth0);
            if(iface.empty())
            {

                iface = "0.0.0.0"; // all local ipv4 addresses
            }
            else
            {
                for(char c : iface)
                {
                    if((c < 48 || c > 57) && c != 46)
                    {
                        // Iterate through chars, if char differs from 0-9 and '.'
                        // we are probably getting a iface name 
                        inet_iface = getIpByIface(iface);
                    }
                }
            }

            sockaddr_in addr;
            int opt = 1;
            
            if(setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
            {
                throw CustomExceptions("Can't set socket");
            }

            addr.sin_family = this->domain;
            addr.sin_port = htons(port);
            
            if (inet_pton(this->domain, &inet_iface[0], &addr.sin_addr.s_addr) < 0)
            {
                
                throw CustomExceptions("Invalid / unsupported Ipv4 address");
            };
            
            if(::bind(this->sock, (sockaddr *)&addr, sizeof(addr)) < 0)
            {
                throw CustomExceptions("Can't bind to port");
            }

            return 0;  
        }
        /*
        Start listening so connections can be accepted
        __maxconns: max amount of connections to queue before refusing
        */
        int listen(int maxconns = 3)
        {
            return ::listen(this->sock, maxconns);
        }
        /*  
            Connection-oriented sockets (TCP) it tries to connect to host:port
            Connectionless sockets (UDP) it sets default host:port for further send() calls(faster than constantly calling sendto())
            Returns 0 on sucess 1 on failure 
        */
        int connect(str_view host, int port)
        {
            sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);

            // Check address and assign it to host struct
            if (inet_pton(this->domain, &host[0], &addr.sin_addr) <= 0)
            {
                return 1;
            }

            // Attempt connecting to host
            if(::connect(this->sock, (sockaddr *)&addr, sizeof(addr)))
            {
                return 1;
            }
            return 0;
        }
        // Attempts connecting to target, addrinfo* struct can be retrieved from getaddrinfo()
        int connect(addrinfo *target)
        {
            if(::connect(this->sock, (sockaddr *)target->ai_addr, target->ai_addrlen))
            {
                return 1;
            }
            return 0;
        }
        // Sends data through socket
        // returns bytes sent
        int send(str_view data)
        {
            return ::send(this->sock, &data[0], data.size(), 0);
        }

        // Sends N bytes of data through socket
        // returns bytes sent
        int send(str_view data, int N)
        {
            return ::send(this->sock, &data[0], N, 0);
        }
        
        // Read data from socket
        str read()
        {
            str data;
            str buffer(BUFF_SIZE, '\x00');
            int last;
            for(;;)
            {
                if((last = ::read(this->sock, &buffer[0], BUFF_SIZE)) < BUFF_SIZE)
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
            ::read(this->sock, &buffer[0], N);
            return buffer;
        }
        
        // Returns all data from socket
        str recv()
        {
            str data;
            str buffer(BUFF_SIZE, '\x00');
            int last;

            for(;;)
            {
                if((last = ::recv(this->sock, &buffer[0], BUFF_SIZE, 0)) < BUFF_SIZE)
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
            ::recv(this->sock, &data[0], N, 0);
            return data;
        }

        // Send 'data' to 'host' on 'port', returns bytes sent
        int sendto(str_view host, int port, str_view data)
        {
            sockaddr_in t;
            t.sin_family = this->domain;
            t.sin_port = htons(port);

            inet_pton(AF_INET, &host[0], &t.sin_addr);

            int n = ::sendto(this->sock, &data[0], data.size(), 0, (struct sockaddr *)&t, sizeof(t));

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

            ::recvfrom(this->sock, &data[0], N, 0, (sockaddr *)&addr, (socklen_t *)&addrlen);

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

        // NOTE -> gsocket::socket(AF_INET, SOCK_STREAM, 0) == SAME AS == gsocket::socket(INET, TCP, BLOCK);
        socket(s_family domain, s_type type, socket_behaviour protocol)
        :__sw(static_cast<int>(domain), (protocol == BLOCK ? static_cast<int>(type) : (static_cast<int>(type) | SOCK_NONBLOCK)), 0)
        {}
    };
    
    class tcp_socket : public __sw
    {
        public:
        tcp_socket(socket_behaviour p = BLOCK)
        :__sw(AF_INET, (static_cast<int>(p) ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)), 0)
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
        tcp_client(str_view host, int port, socket_behaviour p = BLOCK)
        :__sw(static_cast<int>(p) ? __tcp_block_sock : __tcp_noblock_sock)
        {
            this->status = !__sw::connect(host, port);
        }

        int connected()
        {
            return this->status;
        }
    };

    class tcp_server : public __sw
    {
        private:
        int __status = 1;

        public:
        tcp_server(str_view addr_iface, int port, int maxconns = 3, socket_behaviour p = BLOCK)
        :__sw(static_cast<int>(p) ? __tcp_block_sock : __tcp_noblock_sock)
        {
            if(__sw::bind(addr_iface, port))
            {
                this->__status = 0;
            };
            if(__sw::listen(maxconns))
            {
                this->__status = 0;
            };
        }

        tcp_server(int port, int maxconns = 3, socket_behaviour p = BLOCK)
        :__sw(static_cast<int>(p) ? __tcp_block_sock : __tcp_noblock_sock)
        {
            if(__sw::bind(port))
            {
                this->__status = 0;
            };
            if(__sw::listen(maxconns))
            {
                this->__status = 0;
            };
        }

        gsocket::socket accept_connection()
        {
            return __sw::accept();
        }

        int up()
        {
            return this->__status;
        }
    };

    class udp_socket : public __sw
    {
        public:
        // socket_behaviour = BLOCK | NOBLOCK
        // defines file descriptor default behaviour
        udp_socket(socket_behaviour p = BLOCK)
        :__sw(static_cast<int>(p) ? __udp_block_sock : __udp_noblock_sock)
        {}
    };

    class udp_client : public __sw
    {
        public:
        // set default host port for future send() calls
        udp_client(str_view host, int port, socket_behaviour p = BLOCK)
        :__sw(static_cast<int>(p) ? __udp_block_sock : __udp_noblock_sock)
        {
            __sw::connect(host, port);
        }
    };

    class udp_server : public __sw
    {
        private:
        int __status = 0;

        public:
        /*
            bind to port and listen in given address/iface
            if addr_iface is empty, 0.0.0.0 will be chosen,
            meaning it will receive data from any local ipv4 address
        */
        udp_server(str_view addr_iface, int port, socket_behaviour p = BLOCK)
        :__sw(static_cast<int>(p) ? __udp_block_sock : __udp_noblock_sock)
        {
            if(!__sw::bind(addr_iface, port))
            {
                this->__status = 1;
            };
            
        }

        // bind to port and listen in any local ipv4 address
        udp_server(int port, socket_behaviour p = BLOCK)
        :__sw(static_cast<int>(p) ? __udp_block_sock : __udp_noblock_sock)
        {
            if(!__sw::bind(port))
            {
                this->__status = 1;
            };
        }

        int binded()
        {
            return this->__status;
        }
    };

    /* 
        Returns 2 AF_LOCAL sockets (tcp/udp) wrapped with gsocket::socket() class 
        socketpair (CHECK NOTES) - https://man7.org/linux/man-pages/man2/socketpair.2.html
        socket domains & types - https://man7.org/linux/man-pages/man2/socket.2.html
    */
    std::pair<gsocket::socket, gsocket::socket> getsocketpair(s_type type, socket_behaviour b = BLOCK)
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