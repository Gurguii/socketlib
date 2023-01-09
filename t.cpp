#include <iostream>
#include "prototype.hh"

int main()
{
    int n{2};

    gsocket::tcp_socket s0;
    s0.connect("127.0.0.1", 8080);
    gsocket::send("holaa", s0);
    s0.close();
}