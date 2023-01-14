#include "../prototype.hh"

using str = std::string;

int main()
{
    gsocket::tcp_server sv("127.0.0.1", 8080); // CHANGE
    if(sv.up())
    {
        printf("[+] - server is up...\nlistening on %s : %i\n", sv.getsockname());
    }
    str data;
    while(1)
    {
        // Await a new connection
        auto cl = sv.accept_connection();
        printf("connection from %s : %i\n", sv.getpeername());
        // begin of connection w client
        while((data = cl.recv()).size())
        {
            printf("%s\n", &data[0]); // std::cout to be able to redirect output and save files - TODO: investigate that behaviour
        }
        // end of connection w client
        printf("[!] - client closed connection\n");
        cl.close();
    }
}