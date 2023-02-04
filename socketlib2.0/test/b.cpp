#include "../src/socket_wrapper.cpp"
#include "../src/socket.cpp"
#include "../tcp_classes/tcp_server.cpp"
#include "../tcp_classes/tcp_client.cpp"
#include "../src/utils.cpp"
#include "../prototype/test.hh"

int main(){
    auto addr = gsocket::utils::getIpByIface("eth0",inet6);
    tcpClient cl(addr, 8080);
    if(cl.fail()){
        std::cerr << "can't connect - " << sockError << "\n";
        return 0;
    }
    cl.send("heyooo");
    cl.close();
}