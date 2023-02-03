#include "udpServer.hh"

udpServer::udpServer(std::string_view addr, uint16_t port, Behaviour b)
:__sw(AF_INET,(b == BLOCK ? SOCK_DGRAM : (SOCK_DGRAM | SOCK_NONBLOCK)),0){
    bind(addr,port);
}

udpServer::udpServer(uint16_t port, Behaviour b)
:__sw(AF_INET,(b == BLOCK ? SOCK_DGRAM : (SOCK_DGRAM | SOCK_NONBLOCK)),0){
    bind(port);
}