#include "tcpsocket.hpp"

namespace gsocket
{
TCPsocket::TCPsocket(int fd):__base_socket(fd){}

TCPsocket TCPsocket::accept(){
  return TCPsocket(::accept4(_fd, nullptr, nullptr,0));
}
int TCPsocket::connect(std::string_view host, uint16_t port){
  int code;
  if(domain == AF_INET)
  {
    sockaddr_in addr{
      .sin_family = AF_INET,
      .sin_port = htons(port)
    };
    if((code = inet_pton(AF_INET, host.data(), &addr.sin_addr)) <= 0){
      return code; 
    };
    return ::connect(_fd, (sockaddr*)(&addr),sizeof(addr));
  }
  if(domain == AF_INET6) 
  {
    sockaddr_in6 addr{
      .sin6_family = AF_INET6,
      .sin6_port = htons(port)
    };
    if((code = inet_pton(AF_INET6, host.data(), &addr.sin6_addr)) <= 0){
      return code;
    }

    return ::connect(_fd, (sockaddr*)(&addr),sizeof(addr));
  }
  return -1;
}

int TCPsocket::bind(std::string_view ipaddr, uint16_t port, int max_conns){
  if(domain == AF_INET)
  {
    sockaddr_in addr{
      .sin_family = AF_INET,
      .sin_port = htons(port),
    };
    if(!inet_pton(AF_INET, ipaddr.data(), &addr.sin_addr)){
      return -1;
    }
    if(setsockopt(_fd,SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &__sockopt_enable, sizeof(__sockopt_enable))){
      return -1;
    }
    if(::bind(_fd,(sockaddr*)(&addr),sizeof(addr))){
      return -1;
    };
    if(::listen(_fd,3)){
      return -1;
    }
    return 0;
  }
  if(domain == AF_INET6)
  {
    sockaddr_in6 addr{
      .sin6_family = AF_INET6,
      .sin6_port = htons(port),
      .sin6_scope_id = (uint32_t)gsocket::getIdByIp(ipaddr)
    };
    if(!inet_pton(AF_INET6, ipaddr.data(), &addr.sin6_addr)){
      return -1;
    } 
    if(setsockopt(_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &__sockopt_enable, sizeof(__sockopt_enable))){
      return -1;
    }
    if(::bind(_fd,(sockaddr*)(&addr),sizeof(addr))){
      return -1;
    }
    if(::listen(_fd,max_conns)){
      return -1;
    }
    return 0; 
  }
  return -1;
}
}
