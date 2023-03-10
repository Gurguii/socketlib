#ifndef UTILS
#define UTILS
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <sys/ioctl.h>

#include <gsocket/src/enums.hh>
struct Addr4{
    std::string host = std::string(16,'\x00');
    uint16_t port;
};

struct Address{
    std::string host = std::string(46,'\x00');
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
    addrinfo* get(){
        return head;
    };
    ~addressInfo(){
        /* free addrinfo memory when object goes out of scope */
        freeaddrinfo(head);
    }
};

namespace gsocket::utils{
    using str = std::string;
    using str_view = std::string_view;
    str getIpByIface(str_view ifa, Domain &&t);
    int getIdByIp(str_view addr);
    addressInfo getaddrinfo(str_view domain, str_view service, socketPreferences &hints);
    std::pair<std::string,std::string> getnameinfo(addressInfo &addr);
    template <typename T> std::pair<T,T> getsocketpair(Type t, Behaviour b);
    constexpr auto availableBytes = [](int fd){
        int ab;
        ioctl(fd,FIONREAD,&ab);
        return ab;
    };
}

#endif