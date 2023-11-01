#include "udp4socket.hpp"
#include "udpsocket.hpp"

using namespace gsocket;
udp4socket::udp4socket(SOCKET_BEHAVIOUR behaviour)
  :UDPsocket(AF_INET,(behaviour == SOCKET_BEHAVIOUR::block
                             ? SOCK_DGRAM
                             : SOCK_DGRAM | SOCK_NONBLOCK),0){}
