#include "prototype.hh"

int main(){ 
    std::string iface = gsocket::getIpByIface("lo", inet6);
    try{
        gsocket::udp_server sv(iface, 8080, inet6, BLOCK);
        auto [host, port] = sv.getsockname();
        printf("listening on %s : %i\n", host.c_str(), port);
        for(;;){
            auto data = sv.recvfrom();
            printf("from %s port %i:\n%s\n\n", data.host.c_str(), data.port, data.msg.c_str());
        }
    }catch(gsocket::CustomExceptions &err){
        fprintf(stderr, "error - %s\n", err.what().c_str());
        return 0;
    }
}
    
    