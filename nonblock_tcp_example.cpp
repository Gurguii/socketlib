#include <iostream>

#include "prototype.hh"

int main()
{   
    gsocket::tcp_socket s1(s_protocol::noblock); // CHANGE to s_protocol::noblock and compare outputs
    while(s1.connect("192.168.1.99", 8080))
    {
        printf("ERRNO %i - retrying...\n", errno);
        sleep(1);
        continue;
    }
    printf("connected\n");
}