#include "../../prototype.hh"

using str = std::string;

int main()
{
    gsocket::udp_server s("127.0.0.1", 8080);
    if(!s.binded())
    {
        printf("Couldn't bind");
        return -1;
    }
    while(1)
    {
        auto sock = s.recvfrom();
        printf("from %s : %i\ndata: %s\n",sock.host, sock.port, &sock.msg[0]);
    }
}