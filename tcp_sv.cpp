#include "prototype.hh"
#include <iostream>

using str = std::string;

int main()
{
    gsocket::tcp_socket sv;
    try
    {
        const char *listen_addr = "127.0.0.1";
        int port = 8080;
        sv.bind(port, listen_addr);
        printf("\nbinded to interface %s port %i\n", sv.getsockname());
    }catch(gsocket::CustomExceptions &err)
    {
        printf("\n%s\n", err.what().c_str());
        return -1;
    }
    sv.listen(1);
    gsocket::tcp_socket cl;
    str data;
    while(1)
    {
        cl = sv.accept_connection();
        printf("\ngot connection FROM %s : %i\n", cl.getsockname());
        while((data = cl.recv()).size())
        {
            printf("received : %s\n", data.c_str());
        }
        printf("\nclient closed connection\n");
        continue;
    }
}   