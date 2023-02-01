#include <iostream>
#include "src/enums.hh"
#include "src/socket_wrapper.cpp"
#include "src/socket.cpp"
#include "src/utils.cpp"

int main(){
    gsocket::Socket cl(inet,udp,BLOCK);
    cl.connect("127.0.0.1", 8080);
    cl.send("wiskonsin de la wiska");
    cl.close();
}