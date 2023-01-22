#include "prototype.hh"
int main()
{
    try{
        gsocket::tcp_server sv(gsocket::getIpByIface("eth0", Ipv6), 80, Ipv6);
        if(!sv.up()){
            fprintf(stderr, "couldn't bind\n");
            return 0;
        }
        printf("listening on %s : %i\n", sv.getsockname());
        std::string data(1024,'\x00');
        for(;;){
            auto client = sv.accept_connection();
            printf("got conn from %s : %i\n", sv.getpeername());
            while((data = client.recv()).size()){
                printf("received: %s\n", &data[0]);
            }
            printf("client closed connection\n");
            client.close();
        }
        return 1;
    }catch(gsocket::CustomExceptions &err){
        fprintf(stderr, "%s\n", &err.what()[0]);
    }
}