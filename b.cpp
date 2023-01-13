#include "prototype.hh"
#include <errno.h>

int main()
{
    gsocket::tcp_socket s(BLOCK);
    s.connect("192.168.1.99", 8080);
    sleep(2);
    s.send("hello cabronsete\n");
    sleep(2);
    printf("resetting\n");
    s.reset();
    sleep(2);
    s.connect("127.0.0.1", 8080);
    s.send("helloooo :)\n");
    s.close();
}