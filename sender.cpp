#include "prototype.hh"
#include <iostream>


using str = std::string;
constexpr int target_port = 8080;

int main()
{
    const int p = 8080;
    int port = 8080;

    gsocket::udp_socket s("127.0.0.1", 8080);
    s.send("hello from gsocket::udp_socket\n");
    s.close();

    gsocket::udp_socket a("127.0.0.1", port);
    a.send("hello again putito\n");
    a.close();

    gsocket::udp_socket b("127.0.0.1", target_port);
    b.send("hello again cabron\n");
    b.close();
}
