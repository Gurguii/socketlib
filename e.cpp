#include <iostream>
#include "prototype.hh"

using str = std::string;

int main()
{
    gsocket::udp_socket sv;
    sv.bind(8080);
    str data;
    while((data = sv.recv()).size())
    {
        printf("received: %s\n", &data[0]);
    }
} 