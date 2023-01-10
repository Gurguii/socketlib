#include "prototype.hh"

using str = std::string;

int main()
{
    gsocket::tcp_socket sv;
    try 
    {
        sv.bind("192.168.1.99", 8080);
        sv.listen(1);
        printf("Listening on %s : %i\n", sv.getsockname());
    }catch(gsocket::CustomExceptions &err)
    {
        printf("\n%s\n", err.what().c_str());
    }
    str data;
    while(1)
    {
        gsocket::tcp_socket cl = sv.accept_connection();
        printf("\ngot connection FROM %s : %i\n", cl.getsockname());
        while((data = cl.recv()).size())
        {
            printf("received : %s\n", data.c_str());
        }
        printf("\nclient closed connection\n");
        continue;
    }
}   