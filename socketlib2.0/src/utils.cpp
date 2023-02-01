#include "utils.hh"

namespace gsocket::utils
{ 
    std::string getIpByIface(std::string_view ifa, Domain &&t = inet){
        ifaddrs *addrs = nullptr;
        getifaddrs(&addrs);
        std::string iface((t == inet ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN), '\x00');
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
    addressInfo getaddrinfo(std::string_view domain, std::string_view service, socketPreferences &hints){
        addrinfo *addrs = nullptr;
        addrinfo h{
            hints.flags,
            static_cast<int>(hints.domain),
            static_cast<int>(hints.type),
            hints.protocol,
            0,
            nullptr,
            nullptr,
            nullptr
        };
        if(::getaddrinfo(domain.data(), service.data(), &h, &addrs)){
            return nullptr;
        };
        return addressInfo(addrs);
    }
    std::pair<std::string,std::string> getnameinfo(addressInfo &addr){
        std::string _host(46,'\x00');
        std::string _serv(46,'\x00');
        if(!::getnameinfo(addr.get()->ai_addr, addr.get()->ai_addrlen, &_host[0], 46, &_serv[0], 46, 0))
        {
            return {};
        }
        return std::make_pair(_host,_serv);
    }
    template <typename T> std::pair<T,T> getsocketpair(Type t, Behaviour b){
        std::pair<int,int> fds;
        ::socketpair(AF_LOCAL,(b == BLOCK ? static_cast<int>(t) : (static_cast<int>(t) | SOCK_NONBLOCK)),0, &fds.first);
        return fds;
    }
}