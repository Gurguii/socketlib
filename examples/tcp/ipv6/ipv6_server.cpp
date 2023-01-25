#include "prototype.hh"

int main()
{   
    try{
        gsocket::tcp_server sv6(gsocket::getIpByIface("eth0", inet6), 80, inet6);
        /*  
            TODO: fix this printf(). Sometimes it works sometimes it gives a segfault
            printf("listening on %s : %i\n", sv6.getsockname());
        */
        std::string buff(1024, '\x00');
        for(;;){
            auto new_client = sv6.accept_connection();
            while((buff = new_client.recv()).size()){
                printf("received: %s\n", buff.c_str());
            }
            printf("connection closed\n");
            new_client.close();
        }
    }catch(gsocket::CustomExceptions &err){
        fprintf(stderr, "%s\n", err.what().c_str());
    }
}