#include "prototype.hh"
int main(){
    const char *iface = "lo"; // CHANGE
    auto host = gsocket::getIpByIface(iface, inet6);
    int port = 8080;
    try{
        gsocket::udp_client ucl(host, port, inet6, BLOCK, iface);
        std::string msg(1024, '\x00');
        for(;;){
            printf("msg: ");
            getline(std::cin, msg);
            if(msg == "close"){
                ucl.send("closing connection\n");
                ucl.close();
            }
            ucl.send(msg);
        }
    }catch(gsocket::CustomExceptions &err){
        fprintf(stderr, "error - %s\n", err.what().c_str());
        return 0;
    }
}