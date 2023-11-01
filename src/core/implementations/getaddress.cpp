#include "../__base_socket.hpp"
namespace gsocket
{
int __base_socket::get_address(addr &buffer){
 sockaddr address;
 uint32_t sda;
 if(::getsockname(_fd, &address, &sda)){
  return -1;
 };
 if(address.sa_family == AF_INET){
  auto buff = addr4{
   .addr = reinterpret_cast<sockaddr_in*>(&address)->sin_addr.s_addr,
   .port = htons(reinterpret_cast<sockaddr_in*>(&address)->sin_port)
  };
  buffer = std::move(buff);
  return 0;
 }else if(address.sa_family == AF_INET6){
  auto buff = addr6{
   .addr = reinterpret_cast<sockaddr_in6*>(&address)->sin6_addr,
   .port = htons(reinterpret_cast<sockaddr_in6*>(&address)->sin6_port)
  };
  buffer = std::move(buff);
  return 0;
 }
 else{return -1;}
}

int __base_socket::get_peer_address(addr &buffer){
 sockaddr address;
 uint32_t sda;
 if(::getpeername(_fd,&address,&sda)){
  return -1;
 }
 if(address.sa_family == AF_INET){
  auto buff = addr4{
   .addr = ((sockaddr_in*)&address)->sin_addr.s_addr,
   .port = htons(((sockaddr_in*)&address)->sin_port)
  };
  buffer = std::move(buff);
  return 0;
}else if(address.sa_family == AF_INET6){
  auto buff = addr6{
   .addr = ((sockaddr_in6*)&address)->sin6_addr,
   .port = htons(((sockaddr_in6*)&address)->sin6_port)
  };
  buffer = std::move(buff);
  return 0;
 }else{return -1;}
}
}
