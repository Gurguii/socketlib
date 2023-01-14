#include "prototype.hh"
#include <errno.h>

using str = std::string;

int main()
{
    gsocket::udp_socket s;
    for(const int &i : {1,2,3})
    {
        s.sendto("192.168.1.99", 8080, "hellowiis from gsocket::udp_socket\n");
    }
}