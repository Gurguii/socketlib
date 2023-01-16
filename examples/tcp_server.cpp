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
    for(;;)
    {
        // Await a new connection
        auto cl = sv.accept_connection();
        // Print client ip and port
        printf("connection from %s : %i\n", sv.getpeername());
        // Beginning of connection w client
        while((data = cl.recv()).size())
        {
            fprintf(stdout, "%s\n", &data[0]);
            //printf("%s\n", &data[0]); // std::cout to be able to redirect output and save files - TODO: investigate that behaviour
        }
        // End of connection w client
        printf("[!] - client closed connection\n");
        // Close file descriptor (basicly sends a tcp packet with fin, ack bits declaring the end of the connection,
        // you can test the difference by comparing end of connection with and w/o the cl.close() line. You will notice
        // when removing it the client will send a fin, ack packet to the server but the server won't send one back, 
        // I think that's what a dangling socket means, cause it won't get out of scope and it's destructor won't be called)
        cl.close();
    }
}