#include "prototype.hh"
#include <memory>

using str = std::string;

int main()
{
    s_preferences fav_socket{
        .family = INET,
        .type = TCP,
        .protocol = 0,
        .flags = AI_CANONNAME
    };

    //auto fav = std::make_shared<s_preferences>(INET, TCP, 0, AI_CANONNAME);

    //std::cout << "addr: " << fav << " val: " << static_cast<int>((*fav).type) << "\n";
    
    auto a = gsocket::getaddrinfo("www.google.com", "443", &fav_socket);

    auto sock = socket(AF_INET, SOCK_STREAM, 0);

    if(connect(sock, a->ai_addr, a->ai_addrlen))
    {
        fprintf(stderr, "couldn't connect to target\n");
    }

    printf("Sucessfully connected :) to target\n");
    
}