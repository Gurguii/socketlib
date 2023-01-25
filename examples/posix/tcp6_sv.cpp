#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <ifaddrs.h>
#include <net/if.h>
#include <iostream>
#include <errno.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <thread>
#include <vector>
#include <array>

constexpr uint8_t TIMEOUT_SECONDS = 20; // CHANGE
constexpr uint8_t MAX_CLIENTS = 2; // CHANGE



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

void handleClient(int client_fd, const char *host, int port){
    sockaddr client;
    socklen_t addrlen = sizeof(client);

    auto pollReq = pollfd{
        .fd = client_fd,
        .events = POLLIN,
    };
    int recvStatus, ret = 0, available_bytes;
    std::string buff(1024, '\x00');
    pollReq.fd = client_fd;
    for(;;){
        ret = poll(&pollReq, 1, 1000 * TIMEOUT_SECONDS);
        if(ret > 0 && (pollReq.revents & POLLIN)){
            // data available
            ioctl(client_fd, FIONREAD, &available_bytes);
            if(buff.size() < available_bytes){
                buff.resize(available_bytes);
            }
            recvStatus = recv(client_fd, &buff[0], buff.size(), 0);
            if(recvStatus == 0){
                printf("[-] Connection closed\n");
                break;
            }
            if(recvStatus == -1){
                fprintf(stderr, "[-] Recv failed - %s\n", strerror(errno));
                return;
            }
            printf("\n%s : %i\n%s\n\n", host, port, buff.c_str());
            //std::cout << host << buff << "\n";
            buff.assign("");
            continue;
        }
        printf("[!] Connection timeout\n");
        send(client_fd, "Connection timeout\n", 20, 0);
        close(client_fd);
        return;
    }
}

int main(int argc, const char *args[])
{
    /* Input checks and value initialization */
    if(argc != 3){
        fprintf(stderr, "Usage: %s <iface> <port>\n", args[0]);
        return 0;
    }

    const char *iface = args[1];
    int port;

    try{
        port = std::stoi(args[2]);
    }catch(const std::invalid_argument &err){
        fprintf(stderr, "port must be a number\n");
        return 0;
    }

    if(port < 2 || port > 65000){
        fprintf(stderr, "port range: 2 - 65535\n");
        return 0;
    }
    
    if(strlen(iface) > 46){
        fprintf(stderr, "maximum ip size: 46\n");
        return 0;
    }
    /* Actual socket stuff */
    int fd = socket(AF_INET6, SOCK_STREAM, 0), opt = 1;
    
    sockaddr_in6 *addr = new sockaddr_in6;
    addr->sin6_family = AF_INET6;
    addr->sin6_port = htons(port);
    addr->sin6_scope_id = if_nametoindex(iface);

    if(!inet_pton(AF_INET6, getname(iface).c_str(), &addr->sin6_addr)){
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

    std::string address(46,'\x00');
    sockaddr sv_data;
    socklen_t addrlen2 = sizeof(sv_data);
    getsockname(fd, &sv_data, &addrlen2);

    inet_ntop(AF_INET6, &((reinterpret_cast<sockaddr_in6*>(&sv_data))->sin6_addr), &address[0], 46);

    printf("listening on %s : %i\n", &address[0], htons(addr->sin6_port));

    std::string buff(1024, '\x00');

    int client_sock, r, available_bytes = 0;
    auto pollReq = pollfd{
        .events = POLLIN
    };

    std::vector<std::thread> ActiveClients;
    for(;;){
        sockaddr client;
        socklen_t addrlen = sizeof(client);
        std::string host(46, '\x00'); 
        /* FIX THIS */
        while(ActiveClients.size() == MAX_CLIENTS){
            printf("max");
            sleep(2);
            continue;
        }
        /* TILL HERE */
        printf("[+] Awaiting new connection\n");
        /* Got new connection */
        client_sock = accept(fd, &client, &addrlen);
        /* Add client to our threads so that it gets handled */
        inet_ntop(AF_INET6, &(reinterpret_cast<sockaddr_in6*>(&client))->sin6_addr, &host[0], 46);
        printf("[+] CONNECTION FROM %s %i\n", host.c_str(), port);
        ActiveClients.emplace_back(std::thread(handleClient, client_sock, reinterpret_cast<const char *>(&host[0]), port));
    }
}   