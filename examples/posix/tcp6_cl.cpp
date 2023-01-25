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

std::string getname(const char *iface_name){
    if(iface_name == nullptr){
        fprintf(stderr, "iface can't be empty\n");
        return nullptr;
    }
    ifaddrs *addrs = nullptr;
    getifaddrs(&addrs);
    std::string addr(46, '\x00');
    for(auto a = addrs; a!=nullptr; a = a->ifa_next){
        if(a->ifa_addr->sa_family == AF_INET6){
            inet_ntop(AF_INET6, &((sockaddr_in6*)(a->ifa_addr))->sin6_addr, &addr[0], INET6_ADDRSTRLEN);
            if(!strcmp(iface_name, a->ifa_name)){
                freeifaddrs(addrs);
                return addr;
            }
        }
    }
    freeifaddrs(addrs);
    return nullptr;
}

constexpr const char *iface = "eth0"; // CHANGE
constexpr int port = 443; // CHANGE

int main()
{
    int fd = socket(AF_INET6, SOCK_STREAM, 0);
    
    sockaddr_in6 addr;
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(port);
    addr.sin6_scope_id = if_nametoindex("eth0");

    if(!(inet_pton(AF_INET6, getname(iface).c_str(), &addr.sin6_addr))){
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
    close(fd);
}   
