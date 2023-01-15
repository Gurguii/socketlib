#include "prototype.hh"

int main()
{
    gsocket::tcp_socket s(BLOCK);
    std::string data;
    printf("about to recv\n");
    sleep(1);
    data = s.recv();
    printf("Got data: %s\n", &data[0]);
    s.close();
}