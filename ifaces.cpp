#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <errno.h>
#include <map>
#include <sys/types.h>
#include <ifaddrs.h>
#include <iostream>
#include <array>
#include <map>

using str = std::string;
//inet_ntop(a->ifa_addr->sa_family,  &((sockaddr_in*)(a->ifa_addr))->sin_addr, iface.data(), INET_ADDRSTRLEN);
int main()
{
    //std::map<char*, str> testin;
    ifaddrs *addrs = nullptr;
    getifaddrs(&addrs);

    const char* user_iface = "eth0";
    str iface(INET_ADDRSTRLEN, '\x00');
    for(auto a = addrs ; a!=nullptr; a = a->ifa_next)
    {
        if(a->ifa_addr->sa_family == AF_INET)
        {
            if(!strcmp(user_iface, a->ifa_name))
            {
                inet_ntop(a->ifa_addr->sa_family, &((sockaddr_in*)(a->ifa_addr))->sin_addr, &iface[0], INET_ADDRSTRLEN);
                printf("[%s] - %s\n", user_iface, iface.c_str());
            }
        }
    }
}