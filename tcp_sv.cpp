#include "prototype.hh"

using str = std::string;

int main()
{
    gsocket::tcp_server sv("127.0.0.1",8080);
    str data;
    printf("listening %s : %i\n", sv.getsockname());
    while(1)
    {
        auto client = sv.accept_connection();
        printf("incoming connection FROM %s : %i\n", sv.getpeername());
        while((data = client.recv()).size())
        {
            printf("Received: %s\n", &data[0]); 
        }
        printf("connection closed\n");
        client.close();
    }
}   