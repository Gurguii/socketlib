#include "../src/socket_wrapper.cpp"
#include "../src/socket.cpp"
#include "../prototype/test.hh"
#include "../udp_classes/udpClient.cpp"
#include "../udp_classes/udpServer.cpp"
int main(){
    udpServer sv("127.0.0.1",8080); // CHANGE
    auto [host,port] = proto::getsockname(&sv).value();
    std::cout << "Listening on " << host << " : " << port << "\n";
    msgFrom info;
    int n;
    for(;;){
        n = sv.awaitDataFrom(info,-1);
        printf("FROM %s %i\ndata: %s\n", info.addr.host.c_str(), info.addr.port, info.msg.c_str());
        info.msg.assign("");
    }
}