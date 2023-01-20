#include <memory>

#include "prototype.hh"

using str = std::string;

int main()
{
    s_preferences s{
        .family = INET,
        .type = TCP,
        .protocol = 0,
        .flags = AI_CANONNAME
    };
    auto a = gsocket::getaddrinfo("www.gurgui.com", "80", &s);
    gsocket::tcp_socket cl;
    if (cl.connect(a))
    {
        printf("couldn't connect to target\n");
        return 0;
    };
    cl.send("GET /secret.txt HTTP/1.1\r\nHost:192.168.1.99:80\r\n\r\n");
    str data = cl.recv();
    printf("response:\n%s\n", &data[0]);
    cl.close();
}