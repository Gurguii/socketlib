#include "../src/socket_wrapper.cpp"
#include "../src/socket.cpp"
#include "../prototype/test.hh"
#include "../udp_classes/udpClient.cpp"
#include "../udp_classes/udpServer.cpp"
int main(){
    udpServer sv(8080);
    auto [host,port] = proto::getsockname(&sv).value();
    std::cout << "Listening on " << host << " : " << port << "\n";
    msgFrom info;
    int n;
    for(;;){
        n = sv.awaitDataFrom(info,-1);
        printf("received data\n");
    }
}