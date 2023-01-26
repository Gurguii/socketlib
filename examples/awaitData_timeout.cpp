#include "prototype.hh"
int main(){
    try{
        gsocket::tcp_server sv("127.0.0.1", 8080);
        printf("listening on %s %i\n", sv.getsockname());
        std::string data;
        for(;;){
            auto newClient = sv.accept_connection();
            printf("CONN FROM %s : %i\n", sv.getpeername());
            /*Call to awaitData with our data string address
              and desired timeout
              Note: default timeout == -1 what means block until data available*/
            if(newClient.awaitData(&data, 4) == -2){
                fprintf(stderr, "client timeout\n");
                newClient.close();
                continue;
            };
            printf("received: %s\n", data.c_str());
            newClient.close();
        }
    }catch(gsocket::CustomExceptions &err){
        fprintf(stderr, "%s\n", err.what().c_str());
        return 0;
    }
}