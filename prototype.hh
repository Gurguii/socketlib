#ifndef GSOCKETS
#define GSOCKETS

#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <typeinfo>

constexpr int BUFF_SIZE = 1024; // read(), recv() default buffer size when reading

namespace sock_types
{
    constexpr int tcp = 0;
    constexpr int udp = 1;
}

namespace sock_behaviour
{
    constexpr int block = 0;
    constexpr int noblock = 1;
}


namespace gsocket
{
    using str = std::string;
    using str_view = std::string_view;

    class CustomExceptions : public std::exception{
        private:
        std::string message;

        public:
        CustomExceptions(std::string msg) : message(msg) {}
        std::string what(){
            return message;
        }
    };

    struct send
    {
        template <typename T> send(str_view data, T &obj)
        {
            ::send(obj.sock, &data[0], data.size(), 0);      
        } 
        template <typename T> send(str_view data, int &bytes, T &obj)
        {
            printf("2\n");
            printf("%i : %i : %i\n", obj.domain, obj.type, obj.protocol);
        }
        template <typename T> send(str_view data, int &&bytes, T&obj)
        {
            printf("3\n");
            printf("%i : %i : %i\n", obj.domain, obj.type, obj.protocol);
        }
    };

    class __sw // POSIX socket wrapper
    {
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



        std::pair<char*, int> getsockname()
        {
            sockaddr_in s;
            int slen = sizeof(s);
            ::getsockname(this->sock, (sockaddr*)&s, (socklen_t*)&slen);
            return {inet_ntoa(s.sin_addr), htons(s.sin_port)};
        }

        std::pair<char*, int> getpeername()
        {
            sockaddr_in s;
            int addrl = sizeof(s);
            ::getpeername(this->sock, (sockaddr *)&s, (socklen_t *)&addrl);
            return {inet_ntoa(s.sin_addr), htons(s.sin_port)};
        }

        void bind(int port)
        {   
            sockaddr_in *addr = new sockaddr_in;
            int *opt = new int(1);

            if(setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeof(*opt)))
            {
                str msg(100, '\x00');
                sprintf(&msg[0], "\nCan't set socket\n", port);
                throw CustomExceptions(msg);
            }

            addr->sin_family = this->domain;
            addr->sin_port = htons(port);
            addr->sin_addr.s_addr = INADDR_ANY;

            if(::bind(this->sock, (sockaddr *)addr, sizeof(*addr)) < 0)
            {
                str msg(100, '\x00');
                sprintf(&msg[0], "\nCan't bind to port %i\n", port);
                throw CustomExceptions(msg);
            }

            delete addr;
            delete opt;
        }

        void bind(int port, str_view interface_addr)
        {
            sockaddr_in *addr = new sockaddr_in;
            int *opt = new int(1);

            if(setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeof(*opt)))
            {
                str msg(100, '\x00');
                sprintf(&msg[0], "\nCan't set socket\n", port);
                throw CustomExceptions(msg);
            }

            addr->sin_family = this->domain;
            addr->sin_port = htons(port);
            if (inet_pton(this->domain, &interface_addr[0], &addr->sin_addr.s_addr) <= 0)
            {
                throw CustomExceptions("\nInvalid / unsupported ip address\n");
            };
            //addr->sin_addr.s_addr = INADDR_ANY;

            if(::bind(this->sock, (sockaddr *)addr, sizeof(*addr)) < 0)
            {
                str msg(100, '\x00');
                sprintf(&msg[0], "\nCan't bind to port %i\n", port);
                throw CustomExceptions(msg);
            }

            delete addr;
            delete opt;
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
        On connection-oriented sockets (TCP) it tries to connect to host:port
        On connectionless sockets (UDP) it sets default host:port for further send() calls(faster than constantly calling sendto())
        */
        int connect(str_view host,  int port)
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
                return 0;
            }

            delete addr;
            return 1;
        }

        /*
            Sends data through socket
        
        int send(str_view data)
        {
            return ::send(this->sock, &data[0], data.size(), 0);
        }
        /*
            Sends n bytes of data through socket
         
        int send(str_view data, int &n)
        {
            return ::send(this->sock, &data[0], n, 0);
        }

        int send(str_view data, int &&n)
        {
            return ::send(this->sock, &data[0], n, 0);
        }

        /*
            Read data from socket
        */
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

        /*
            Read n bytes of data from socket
        */
        str read(int &bytes)
        {
            str buffer(BUFF_SIZE, '\x00');
            ::read(this->sock, &buffer[0], bytes);
            return buffer;
        }

        str read(int &&bytes)
        {
            str buffer(BUFF_SIZE, '\x00');
            ::read(this->sock, &buffer[0], bytes);
            return buffer;
        }
        // Returns all data available in socket
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

        struct recv
        {
            recv(int &bytes)
            {
                
            }
            recv(int &&bytes)
            {
                
            }
        };

        // Returns n bytes of data from socket
        //str recv(int &bytes)
        //{
        //    str data(bytes,'\x00');
        //    ::recv(this->sock, &data[0], bytes, 0);
        //    return data;
        //}
//
        //str recv(int &&bytes)
        //{
        //    str data(bytes,'\x00');
        //    ::recv(this->sock, &data[0], bytes, 0);
        //    return data;
        //}
    
        // Send 'data' to 'host' on 'port', returns bytes sent
        int sendto(str_view host, int port, str_view data)
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

        socket(int domain, int type, int protocol)
        :__sw(domain, type, protocol)
        {
            // socket constructor
        }

        socket(int s)
        :__sw(s)
        {
            /* 
                socket() wrapper 
            */
        }
    };

    class tcp_socket : public __sw
    {
        protected:
        tcp_socket(int &&sock)
        :__sw(sock)
        {
            /* 
                socket() wrapper
            */
        }

        public:
        tcp_socket()
        :__sw(AF_INET, SOCK_STREAM, 0)
        {
            // Default constructor
        }

        tcp_socket(str_view host, int &port)
        :__sw(AF_INET, SOCK_STREAM, 0)
        {
            __sw::connect(host, port);
        }

        tcp_socket(str_view host, int &&port)
        :__sw(AF_INET, SOCK_STREAM, 0)
        {
            __sw::connect(host, port);
        }
        
        tcp_socket(int &port, int &maxconns)
        :__sw(AF_INET, SOCK_STREAM, 0)
        {
            // Bind socket to port queueing a max amount of 'maxconns' connections
            __sw::bind(port);
            __sw::listen(maxconns);
        }
        
        tcp_socket(int &&port, int &&maxconns)
        :__sw(AF_INET, SOCK_STREAM, 0)
        {   
            __sw::bind(port);
            __sw::listen(maxconns);
        }

        tcp_socket accept_connection()
        {
            return tcp_socket(__sw::accept());
        }

    };

    class udp_socket : public __sw
    {
        public:
        udp_socket()
        :__sw(AF_INET, SOCK_DGRAM, 0)
        {
            // Default constructor
            if(sock)
            {
                this->sock = sock;
            }
        };
        
        
        udp_socket(int &port)
        :__sw(AF_INET, SOCK_DGRAM, 0)
        {
            // Bind socket to 'port'
            __sw::bind(port);
        }
        
        udp_socket(int &&port)
        :__sw(AF_INET, SOCK_DGRAM, 0)
        {
            // Bind socket to 'port'
            __sw::bind(port);
        }

        udp_socket(str_view host, int &port)
        :__sw(AF_INET, SOCK_DGRAM, 0)
        {
            // Set default host, port for send() calls
            __sw::connect(host, port);
        }

        udp_socket(str_view host, int &&port)
        :__sw(AF_INET, SOCK_DGRAM, 0)
        {
            // Set default host, port for send() calls
            printf("port rvalue\n");
            __sw::connect(host, port);
        }

        udp_socket(str_view host, const int &port)
        :__sw(AF_INET, SOCK_DGRAM, 0)
        {
            // Set default host, port for send() calls
            __sw::connect(host, port);
        }
    };


    /* 
        Returns 2 AF_LOCAL sockets (tcp/udp) wrapped with gsocket::socket() class 
        https://man7.org/linux/man-pages/man2/socketpair.2.html
        socket domains & types - https://man7.org/linux/man-pages/man2/socket.2.html
    */
    std::pair<gsocket::socket, gsocket::socket> getsocketpair(int type)
    {   
        if(type != sock_types::tcp && type != sock_types::udp)
        {
            throw CustomExceptions("Socketpair type must be tcp/udp\n");
        }

        std::pair<int, int>fds;
        if(::socketpair(AF_LOCAL, (type == sock_types::tcp ? SOCK_STREAM : SOCK_DGRAM), 0, &fds.first))
        {
            throw CustomExceptions("Couldn't create socketpair");
        }
        return {gsocket::socket(fds.first), gsocket::socket(fds.second)}; 
    }
}
#endif