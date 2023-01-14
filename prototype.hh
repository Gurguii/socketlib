#ifndef GSOCKETS
#define GSOCKETS

#include <string.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <ifaddrs.h>
#include <errno.h> // NOT IN USE

#define __tcp_block_sock socket(AF_INET, SOCK_STREAM, 0)
#define __tcp_noblock_sock socket(AF_INET, (SOCK_STREAM | SOCK_NONBLOCK), 0)
#define __udp_block_sock socket(AF_INET, SOCK_DGRAM, 0)
#define __udp_noblock_sock socket(AF_INET, (SOCK_DGRAM | SOCK_NONBLOCK), 0)

constexpr int BUFF_SIZE = 1024; // read(), recv() default buffer size when reading
/*
    Some recv() flags:
    
    MSG_WAITFORONE - wait for at least one packet to return
*/

enum struct s_domains : uint8_t
{
    local = 1, // AF_LOCAL == AF_UNIX
    #define LOCAL s_domains::local
    inet = 2 // AF_INET
    #define INET s_domains::inet
};

// defines possible sock types
// when calling gsocket::getsocketpair()
enum struct s_types : uint8_t
{   
    tcp = 1, // SOCK_STREAM
    #define TCP s_types::tcp
    udp = 2 // SOCK_DGRAM
    #define UDP s_types::udp
    
};

enum struct socket_behaviour : uint8_t
{
    block = 1, // BLOCKING FILE DESCRIPTOR
    #define BLOCK socket_behaviour::block
    noblock = 0 // NONBLOCKING FILE DESCRIPTOR
    #define NOBLOCK socket_behaviour::noblock
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
        
        // idk if this is worth, the idea is to avoid constructing a new obj and just
        // closing and creating a new sock on the already existing obj
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
                // all local ipv4 addresses
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
                throw CustomExceptions("Couldn't set socket");
            }

            addr.sin_family = this->domain;
            addr.sin_port = htons(port);
            
            if (inet_pton(this->domain, &inet_iface[0], &addr.sin_addr.s_addr) < 0)
            {
                
                throw CustomExceptions("Invalid / unsupported Ipv4 address");
            };
            
            if(::bind(this->sock, (sockaddr *)&addr, sizeof(addr)) < 0)
            {
                throw CustomExceptions("Couldn't bind to port");
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
        */
        int connect(str_view host, int port)
        {
            sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);

            // Check address and assign it to host struct
            if (inet_pton(AF_INET, &host[0], &addr.sin_addr) <= 0)
            {
                throw CustomExceptions("\nInvalid / unsupported ip address\n");
            }

            // Attempt connecting to host
            if(::connect(this->sock, (sockaddr *)&addr, sizeof(addr)))
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
        socket(s_domains domain, s_types type, socket_behaviour protocol)
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
        protected:
        int __connected = 0;

        public:
        
        tcp_client(str_view host, int port, socket_behaviour p = BLOCK)
        :__sw(static_cast<int>(p) ? __tcp_block_sock : __tcp_noblock_sock)
        {
            this->__connected = !__sw::connect(host, port);
        }

        int connected()
        {
            return this->__connected;
        }
    };

    class tcp_server : public __sw
    {
        protected:
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
            __sw::bind(port);
            __sw::listen(maxconns);
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
        protected:
        int __binded = 0;

        public:
        // bind to port and listen in given address/iface
        udp_server(str_view addr_iface, int port, socket_behaviour p = BLOCK)
        :__sw(static_cast<int>(p) ? __udp_block_sock : __udp_noblock_sock)
        {
            if(!__sw::bind(addr_iface, port))
            {
                this->__binded = 1;
            };
            
        }

        // bind to port and listen in any local address
        udp_server(int port, socket_behaviour p = BLOCK)
        :__sw(static_cast<int>(p) ? __udp_block_sock : __udp_noblock_sock)
        {
            if(!__sw::bind(port))
            {
                this->__binded = 1;
            };
        }

        int up()
        {
            return this->__binded;
        }
    };

    /* 
        Returns 2 AF_LOCAL sockets (tcp/udp) wrapped with gsocket::socket() class 
        socketpair (CHECK NOTES) - https://man7.org/linux/man-pages/man2/socketpair.2.html
        socket domains & types - https://man7.org/linux/man-pages/man2/socket.2.html
    */
    std::pair<gsocket::socket, gsocket::socket> getsocketpair(s_types type, socket_behaviour b = BLOCK)
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