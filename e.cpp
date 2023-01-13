#include <iostream>
#include "prototype.hh"

int main()
{
    gsocket::udp_socket s;
    for(const int &i : {1,2,3})
    {
        s.sendto("127.0.0.1", 8080, "hellouuuu");
    }
    
}