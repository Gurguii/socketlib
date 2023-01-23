#include "prototype.hh"

int main()
{
    s_preferences pref{
    .family = inet,
    .type = stream,
    .protocol = 0,
    .flags = AI_CANONNAME | AI_ALL
    };

    auto ai = gsocket::getaddrinfo("www.gurgui.com", "80", &pref);
    gsocket::tcp_client s(ai.get());
    if(s.failure()){
        fprintf(stderr, "couldn't connect\n");
        return 0;
    }
    
    s.send("GET /secret.txt HTTP/1.1\r\nHost:192.168.1.99\r\n\r\n");
    std::string data (1024, '\x00');
    data = s.recv();
    printf("received: %s\n", data.c_str());
    s.close();
}