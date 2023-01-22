#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <ifaddrs.h>
#include <netdb.h>
#include <net/if.h>
#include <errno.h> // NOT IN USE
#include <iostream> // TEMP

char* getname(const char *iface_name){
    ifaddrs *addrs = nullptr;
    getifaddrs(&addrs);
    char *myaddr = new char(46);
    for(auto a = addrs; a!=nullptr; a = a->ifa_next){
        if(a->ifa_addr->sa_family == AF_INET6){
            inet_ntop(AF_INET6, &((sockaddr_in6*)(a->ifa_addr))->sin6_addr, myaddr, INET6_ADDRSTRLEN);
            if(!strcmp(iface_name, a->ifa_name)){
                freeifaddrs(addrs);
                return myaddr;
            }
        }
    }
    freeifaddrs(addrs);
    return nullptr;
}

int main()
{
    int fd = socket(AF_INET6, SOCK_STREAM, 0);
    sockaddr_in6 addr;
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(80);
    addr.sin6_scope_id = if_nametoindex("eth0");
    if(!inet_pton(AF_INET6, getname("eth0"), &addr.sin6_addr)){
        fprintf(stderr, "couldn't assign address\n");
        return 0;
    };

    if(connect(fd,(sockaddr *)&addr, (socklen_t)(sizeof(addr)))){
        fprintf(stderr, "couldn't connect\n");
        return 0;
    }   
    printf("[+] - connected\n");
    std::string msg(1024, '\x00');
    for(;;){
        printf("msg: ");
        getline(std::cin, msg);
        if(msg == "close"){
            close(fd);
            return 0;
        }
        send(fd, &msg[0], msg.size(), 0);
    }
}   
