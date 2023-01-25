#include "prototype.hh"

void handler(Client* client){
    auto wrappedClient = gsocket::socket((*client).fd);
    try{
        std::string data;
        while((data = wrappedClient.recv()).size()){
            printf("data: %s\n", data.c_str());
        }

    }catch(gsocket::CustomExceptions &err){
        fprintf(stderr, "error - %s\n", err.what().c_str());
        return;
    }
}

int main(){
    gsocket::multiTcpServer msv("192.168.1.99", 8080);
    printf("listening on %s : %i\n", msv.getsockname());
    msv.start(&handler);
}