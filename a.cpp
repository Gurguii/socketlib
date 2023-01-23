#include "prototype.hh"
#include <thread>

int main()
{
    s_preferences pref{
        .family = inet,
        .type = stream,
        .protocol = 0,
        .flags = 0
    };

    auto ai = gsocket::getaddrinfo("www.gurgui.com", "80", &pref);
    auto [addr, addrlen] = ai.get_connect();

    gsocket::tcp_client s(addr, addrlen);
    if(s.connected()){
        printf("got conn\n");
        s.send("GET /secret.txt HTTP/1.1\r\nHost:192.168.1.99\r\n\r\n");
        std::string data (1024, '\x00');
        data = s.recv();
        printf("received: %s\n", data.c_str());
    }
}