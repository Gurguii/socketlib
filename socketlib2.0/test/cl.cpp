#include <iostream>
#include "../src/enums.hh"
#include "../src/socket_wrapper.cpp"
#include "../src/socket.cpp"
#include "../src/utils.cpp"
#include "../tcp_classes/tcp_socket.cpp"
#include "../tcp_classes/tcp_client.cpp"
int main(){
    Client cl{
        .host{"127.0.0.1"},
        .port{8080},
        .dom{inet},
        .beh{BLOCK}
    };
    gsocket::tcpClient u(cl);
    if(u.fail()){
        std::cerr << "can't connect\n";
    }
    u.send("hello\n");
}