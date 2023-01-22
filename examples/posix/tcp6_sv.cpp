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
    int fd = socket(AF_INET6, SOCK_STREAM, 0), opt = 1;

    sockaddr_in6 *addr = new sockaddr_in6();
    addr->sin6_family = AF_INET6;
    addr->sin6_port = htons(443);
    addr->sin6_scope_id = if_nametoindex("eth0");

    if(!inet_pton(AF_INET6, getname("eth0"), &addr->sin6_addr)){
        fprintf(stderr, "couldn't assign address\n");
        return 0;
    };
    
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        fprintf(stderr, "couldn't set socket\n");
        return 0;
    }

    if(bind(fd, (sockaddr *)addr, INET6_ADDRSTRLEN)){
        fprintf(stderr, "couldn't bind\n");
        return 0;
    };

    if(listen(fd, 3)){
        fprintf(stderr, "couldn't start listening\n");
        return 0;
    }

    char *address = new char(46);
    sockaddr sv_data;
    socklen_t addrlen2 = sizeof(sv_data);

    getsockname(fd, &sv_data, &addrlen2);
    inet_pton(AF_INET6, sv_data.sa_data, address);
    printf("listening on %s : %i\n", address, htons(addr->sin6_port));
    std::string buff(1024, '\x00');
    sockaddr client_data;
    socklen_t addrlen = sizeof(client_data);
    int r;
    for(;;){
        auto client_fd = accept(fd, &client_data, &addrlen);
        printf("got connection\n");
        for(;;){
            if(r = recv(client_fd, &buff[0], buff.size(), 0) <= 0){
                printf("client closed connection\n");
                close(client_fd);
                break;
            };
            printf("received: %s\n", &buff[0]);
            buff.clear();
        }

    }
}