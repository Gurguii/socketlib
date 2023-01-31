#ifndef UTILS
#define UTILS

#include "socket_wrapper.hh"
//#include <string>
//#include <cstring>
//#include <arpa/inet.h>
//#include <ifaddrs.h>
//
//#include "enums.hh"
struct socketPreferences{
    Domain domain;
    Type type;
    int protocol;
    int flags;
};

struct addressInfo{
    private:
    addrinfo *const head;

    public:
    addressInfo(addrinfo *addrs):head(addrs){}
    addrinfo *const get(){
        return head;
    };
    std::pair<std::string,std::string> retrieve(){
        return std::make_pair<std::string,std::string>("","");
    }
    ~addressInfo(){
        std::cout << "~ addressInfo destructor ...\n";
        /* free addrinfo memory when object goes out of scope */
        freeaddrinfo(head);
    }
};

namespace gsocket::utils{
    std::string getIpByIface(std::string_view ifa, Domain &&t);
    std::string getaddrinfo(std::string_view a, std::string_view b);
}







#endif