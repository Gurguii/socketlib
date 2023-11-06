#include "tcp4socket.hpp"
#include "tcpsocket.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>

namespace gsocket
{
tcp4socket::tcp4socket(int fd):TCPsocket(fd){
  domain = AF_INET;
}

// @default ACTION none
// @default BEHAVIOUR block

tcp4socket::tcp4socket(SOCKET_BEHAVIOUR behaviour)
  :TCPsocket(socket(AF_INET,(behaviour == SOCKET_BEHAVIOUR::block ? SOCK_STREAM : SOCK_STREAM | SOCK_NONBLOCK),0)){
  domain = AF_INET;
}

tcp4socket tcp4socket::accept(){
  return ::accept(_fd,nullptr,0);
}

tcp4socket tcp4socket::accept(addr &address){
  addr4 _add;
  tcp4socket client = accept(_add);
  address = std::move(_add);
  return client;
  //sockaddr_in addr;
  //socklen_t addrlen = sizeof(addr);
  //tcp4socket client = ::accept4(_fd,(sockaddr*)&addr,&addrlen,0);
  //addr4 _add{
  //  .addr = addr.sin_addr.s_addr,
  //  .port = htons(addr.sin_port)
  //};
  //address = std::move(_add);
  //return client;
};

tcp4socket tcp4socket::accept(addr4 &address){
  sockaddr_in addr;
  uint32_t addrlen = sizeof(addr);
  tcp4socket incoming_socket = ::accept(_fd,(sockaddr*)&addr,&addrlen);
  address.addr = std::move(addr.sin_addr.s_addr);
  address.port = htons(std::move(addr.sin_port));
  return incoming_socket;
}
}
