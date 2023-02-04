#ifndef UTILS
#define UTILS
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <sys/ioctl.h>

#include <gsocket/enums.hh>
struct Address{
    std::string host;
    uint16_t port;
};

struct msgFrom{
    Address addr;
    std::string msg;
};

struct socketPreferences{
    int flags;
    Domain domain;
    Type type;
    int protocol;
};

struct addressInfo{
    private:
    addrinfo *head;

    public:
    addressInfo(addrinfo *addrs):head(addrs){}
    const addrinfo* get(){
        return head;
    };
    std::pair<std::string,int> retrieve(){
        auto addr = reinterpret_cast<sockaddr_in*>(&head->ai_addr);
        return std::make_pair<std::string,int>(inet_ntoa(addr->sin_addr),htons(addr->sin_port));
    }
    ~addressInfo(){
        /* free addrinfo memory when object goes out of scope */
        freeaddrinfo(head);
    }
};

namespace gsocket::utils{
    using str = std::string;
    using str_view = std::string_view;
    str getIpByIface(str_view ifa, Domain &&t);
    addressInfo getaddrinfo(str_view domain, str_view service, socketPreferences &hints);
    std::pair<str,str> getnameinfo(addressInfo &addr);
    template <typename T> std::pair<T,T> getsocketpair(Type t, Behaviour b);
    constexpr auto availableBytes = [](int fd){
        int ab;
        ioctl(fd,FIONREAD,&ab);
        return ab;
    };
}

#endif