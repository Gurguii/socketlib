#ifndef SOCKET_WRAPPER_HH
#define SOCKET_WRAPPER_HH
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <ifaddrs.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <optional>

#define sockError strerror(errno)
#define __IO_BUFFSIZE 1024

#include <gsocket/enums.hh>
#include <gsocket/utils>

namespace gsocket{
    using ui16 = uint16_t;
    using ui8 = uint8_t;
    using str = std::string;
    using str_view = std::string_view;
    class __sw{
        protected:
        __sw(Domain d, Type t, Behaviour b);
        __sw(ui8 domain, ui8 type, ui8 protocol);
        __sw(ui8 fd);
        ~__sw();
        public:
        ui8 domain,type,fd;
        /* GET FILE DESCRIPTOR */
        const int getFD(){
            return fd;
        }
        /* CLOSE */
        int close();
        /* CONNECT */
        int connect(str_view host, uint16_t port);
        int connect(addrinfo *const addr);
        int connect(addressInfo *addr);
        int connect(sockaddr *addr,socklen_t &addrlen);
        /* SEND DATA */
        int send(str_view data);
        int send(str_view data, int bytes);
        int sendto(str_view host, uint16_t port, str_view msg);
        /* RECV DATA */
        std::optional<str> recv();
        std::optional<str> recv(int bytes);
        int recvfrom(msgFrom &data);
        /* AWAIT DATA */
        template <typename T> int awaitData(T &buffer, int timeout);
        int awaitDataFrom(msgFrom &__sockHostData, int timeout);
        /* BIND */
        int bind(str_view addr, uint16_t port);
        int bind(uint16_t port);
        /* LISTEN */
        int listen(int maxConns);
        /* ACCEPT */
        int accept();
        int accept(Address& addr);
        /* RETRIEVE SOCKET DATA */
        Address getsockname();
        Address getpeername();
    };
}
#endif