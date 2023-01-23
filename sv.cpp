#include "prototype.hh"

int main(){
    try{
        auto sv = gsocket::tcp_server("192.168.1.99", 8080);
        printf("listening on %s : %i\n", sv.getsockname());
        std::string buffer;
        for(;;){
            auto nclient = sv.accept_connection();
            printf("got conn %s : %i\n", sv.getpeername());
            while((buffer = nclient.recv()).size()){
                printf("received: %s\n", &buffer[0]);
            }
            printf("client closed connection\n");
            nclient.close();
        }
    }catch(gsocket::CustomExceptions &err){
        fprintf(stderr, "%s\n", &err.what()[0]);
        return 0;
    }
}