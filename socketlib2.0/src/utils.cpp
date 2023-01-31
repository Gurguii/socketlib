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
    std::string getaddrinfo(std::string_view a, std::string_view b){
        std::cout << a << " " << b << "\n";
    }
}
