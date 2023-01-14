#include "prototype.hh"

using str = std::string;

int main()
{
    gsocket::udp_socket s;
    s.connect("127.0.0.1", 8080);
    s.send("esporádico\n");
    s.close();
}