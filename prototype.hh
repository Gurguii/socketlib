#ifndef GSOCKETS
#define GSOCKETS

#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <errno.h>
#include <ifaddrs.h>

constexpr int BUFF_SIZE = 1024; // read(), recv() default buffer size when reading

enum struct s_domains : uint8_t
{
    local = 1, // AF_LOCAL == AF_UNIX
    #define LOCAL s_domains::local
    inet = 2 // AF_INET
    #define INET s_domains::inet
};

enum struct s_types : uint8_t
{   
    tcp = 1, // SOCK_STREAM
    #define TCP s_types::tcp
    udp = 2 // SOCK_DGRAM
    #define UDP s_types::udp
    
};

enum struct s_protocol : uint8_t
{
    block = 1, // BLOCKING FILE DESCRIPTOR or OPERATION
    #define BLOCK s_protocol::block
    noblock = 0 // NONBLOCKING FILE DESCRIPTOR or OPERATION
    #define NOBLOCK s_protocol::noblock
};

namespace gsocket
{
    using str = std::string;
    using str_view = std::string_view;

    // Returns ipv4 addr of interface E.g eth0, lo, docker0
    str getIpByIface(str_view ifa)
    {
        ifaddrs *addrs = nullptr;
        getifaddrs(&addrs);

        str iface(INET_ADDRSTRLEN, '\x00');

        for(auto a = addrs ; a!=nullptr; a = a->ifa_next)
        {
            if(a->ifa_addr->sa_family == AF_INET)
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

        // Bind to port ready to listen in any address(interfaces)
        bool bind(int port)
        {
            sockaddr_in *addr = new sockaddr_in;
            int *opt = new int(1);

            if(setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeof(*opt)))
            {
                this->error="Can't set socket";
                return true;
            }

            addr->sin_family = this->domain;
            addr->sin_port = htons(port);
            addr->sin_addr.s_addr = INADDR_ANY;

            if(::bind(this->sock, (sockaddr *)addr, sizeof(*addr)) < 0)
            {
                this->error="Can't bind to port ";
                return true;
            }

            delete addr;
            delete opt;
            return false;
        }

        // Binds socket to 
        bool bind(str_view iface, int port)
        {
            str inet_iface{iface};
            // Check if iface is an address or iface name (E.g eth0);
            if(iface.empty())
            {
                iface = "0.0.0.0";
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
                this->error = "Can't set socket";
                return true;
            }

            addr.sin_family = this->domain;
            addr.sin_port = htons(port);
            
            if (inet_pton(this->domain, &inet_iface[0], &addr.sin_addr.s_addr) < 0)
            {
                this->error = "Invalid / unsupported ip address";
                return true;
            };
            
            if(::bind(this->sock, (sockaddr *)&addr, sizeof(addr)) < 0)
            {
                this->error = "Couldn't bind to port";
                return true;
            }

            return false;  
        }
        /*
        Start listening so connections can be accepted
        __maxconns: max amount of connections to queue before refusing
        */
        void listen(int maxconns = 3)
        {
            if(::listen(this->sock, maxconns) < 0)
            {
                throw CustomExceptions("\nSocket couldn't start listening\n");
            }
        }

        /*  
            Connection-oriented sockets (TCP) it tries to connect to host:port
            Connectionless sockets (UDP) it sets default host:port for further send() calls(faster than constantly calling sendto())
        */
        int connect(str_view host, int port)
        {
            sockaddr_in *addr = new sockaddr_in;
            addr->sin_family = AF_INET;
            addr->sin_port = htons(port);

            // Check address and assign it to host struct
            if (inet_pton(AF_INET, &host[0], &addr->sin_addr) <= 0)
            {
                throw CustomExceptions("\nInvalid / unsupported ip address\n");
            }

            // Attempt connecting to host
            if(::connect(this->sock, (sockaddr *)addr, sizeof(*addr)))
            {
                delete addr;
                return 1;
            }

            delete addr;
            return 0;
        }

        // Sends data through socket
        // returns bytes sent
        int send(str_view data, s_protocol p = BLOCK)
        {
            return ::send(this->sock, &data[0], data.size(), static_cast<int>(p));
        }

        // Sends N bytes of data through socket
        // returns bytes sent
        int send(str_view data, int N, s_protocol p = BLOCK)
        {
            return ::send(this->sock, &data[0], N, static_cast<int>(p));
        }
        
        // Read data from socket
        str read()
        {
            str data;
            str buffer(BUFF_SIZE, '\x00');
            int last;
            while(1)
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
        template <typename T> str read(T N)
        {
            str buffer(N, '\x00');
            ::read(this->sock, &buffer[0], N, 0);
            return buffer;
        }
        
        // Returns all data from socket
        str recv()
        {
            str data;
            str buffer(BUFF_SIZE, '\x00');
            int last;

            while(1)
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
        template <typename T> str recv(T N)
        {
            str data(N, '\x00');
            ::recv(this->sock, &data[0], N, 0);
            return data;
        }

        // Send 'data' to 'host' on 'port', returns bytes sent
        template <typename T> int sendto(str_view host, T port, str_view data)
        {
            sockaddr_in *t = new sockaddr_in;
            t->sin_family = this->domain;
            t->sin_port = htons(port);

            inet_pton(AF_INET, &host[0], &t->sin_addr);

            int n = ::sendto(this->sock, &data[0], data.size(), 0, (struct sockaddr *)t, sizeof(*t));
            delete t;

            return n;
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
        // more info absocket() - https://man7.org/linux/man-pages/man2/socket.2.html
        socket(int domain, int type, int protocol)
        :__sw(domain, type, protocol)
        {}

        // NOTE -> gsocket::socket(AF_INET, SOCK_STREAM, 0) == SAME AS == gsocket::socket(INET, TCP, BLOCK);
        socket(s_domains domain, s_types type, s_protocol protocol)
        :__sw(static_cast<int>(domain), (protocol == BLOCK ? static_cast<int>(type) : (static_cast<int>(type) | SOCK_NONBLOCK)), 0)
        {}
    };
    
    class tcp_socket : public __sw
    {
        public:
        tcp_socket(s_protocol p = BLOCK)
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
        public:
        tcp_client(str_view host, int port, s_protocol p = BLOCK)
        :__sw(AF_INET, (static_cast<int>(p) ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)), 0)
        {
            __sw::connect(host, port);
        }
    };

    class tcp_server : public __sw
    {

        public:
        tcp_server(str_view addr_iface, int port, int maxconns = 3, s_protocol p = BLOCK)
        :__sw(AF_INET, (static_cast<int>(p) ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)), 0)
        {
            __sw::bind(addr_iface, port);
            __sw::listen(maxconns);
        }

        tcp_server(int port, int maxconns = 3, s_protocol p = BLOCK)
        :__sw(AF_INET, (static_cast<int>(p) ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)), 0)
        {
            __sw::bind(port);
            __sw::listen(maxconns);
        }

        gsocket::socket accept_connection()
        {
            return __sw::accept();
        }
    };
    // UDP CLIENT && UDP SERVER
    /* 
        Returns 2 AF_LOCAL sockets (tcp/udp) wrapped with gsocket::socket() class 
        POSIX socketpair - https://man7.org/linux/man-pages/man2/socketpair.2.html
        socket domains & types - https://man7.org/linux/man-pages/man2/socket.2.html
    */
    std::pair<gsocket::socket, gsocket::socket> getsocketpair(s_types type)
    {   
        std::pair<int, int>fds;
        if(::socketpair(AF_LOCAL, static_cast<int>(type), 0, &fds.first))
        {
            throw CustomExceptions("Couldn't create socketpair");
        }
        return {gsocket::socket(fds.first), gsocket::socket(fds.second)}; 
    }
}
#endif