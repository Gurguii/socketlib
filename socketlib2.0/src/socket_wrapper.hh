#ifndef SOCKET_WRAPPER
#define SOCKET_WRAPPER
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <ifaddrs.h>
#include <netdb.h>
#include <net/if.h>
//#include <errno.h>
//#include <memory>
//#include <thread>
#include <sys/ioctl.h>
#include <sys/poll.h>
//#include <vector>
//#include <variant>
//#include <mutex>
#include <optional>

#define sockError strerror(errno)
#define __IO_BUFFSIZE 1024

#include "enums.hh"
#include "utils.hh"

namespace gsocket{
    /* WHAT TO DO? */
    //using ui16 = uint16_t;
    //using ui8 = uint8_t;
    //using str = std::string;
    //using str_view = std::string;
    class __sw{
        private:
        uint8_t domain,type,fd;
        protected:
        __sw(Domain d, Type t, Behaviour b);
        __sw(uint8_t domain, uint8_t type, uint8_t protocol);
        __sw(uint8_t fd);
        ~__sw(){
            close();
        }
        public:
        /* GET FILE DESCRIPTOR */
        const int getFD(){
            return fd;
        }
        /* CLOSE */
        int close();
        /* CONNECT */
        int connect(std::string_view host, uint16_t port);
        int connect(addrinfo *const addr);
        int connect(addressInfo *addr);
        /* SEND DATA */
        int send(std::string_view data);
        int send(std::string_view data, int bytes);
        int sendto(std::string_view host, uint16_t port, std::string_view msg);
        /* RECV DATA */
        std::optional<std::string> recv();
        std::optional<std::string> recv(int bytes);
        int recvfrom(msgFrom &data);
        /* AWAIT DATA */
        template <typename T> int awaitData(T &buffer, int timeout);
        int awaitDataFrom(msgFrom &__sockHostData, int timeout);
        /* BIND */
        int bind(std::string_view addr, uint16_t port);
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