#include "prototype.hh"

using str = std::string;

int main()
{
    s_preferences fav{
        .family = INET,
        .type = TCP,
        .protocol = 0,
        .flags = AI_CANONNAME
    };

    auto a = gsocket::getaddrinfo("www.google.com", "https", &fav);
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    for(;;)
    {
        if(connect(fd, a->ai_addr, a->ai_addrlen))
        {
            fprintf(stderr, "couldn't connect [%i], retrying...\n", errno);
            sleep(1);
            continue;
        }   
        break;
    }
    printf("succesfully connected :)\n");
    str msg;
    for(;;)
    {
        printf("send: ");
        getline(std::cin, msg);
        if(msg == "close")
        {
            printf("closing connection...\n");
            close(fd);
            break;
        }
        send(fd, &msg[0], msg.size(), 0);
    }
}