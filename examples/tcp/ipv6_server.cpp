#include "prototype.hh"
int main()
{
    gsocket::tcp_server sv(gsocket::getIpByIface("lo", inet6), 8080, inet6);
    if(!sv.up()){
        fprintf(stderr, "couldn't bind\n");
        return 0;
    }
    printf("listening on %s : %i\n", sv.getsockname());
    std::string data(1024,'\x00');
    for(;;){
        auto client = sv.accept_connection();
        while((data = client.recv()).size()){
            printf("received: %s\n", &data[0]);
        }
        printf("client closed connection\n");
        client.close();
    }
    return 1;
}