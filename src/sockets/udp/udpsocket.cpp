#include "udpsocket.hpp"

#define _XOPEN_SOURCE_EXTENDED 1
constexpr int __ENABLE = 1;
constexpr int __DISABLE = 0;

namespace gsocket
{
  UDPsocket::UDPsocket(int fd):__base_socket(fd){}
  UDPsocket::UDPsocket(int domain, int type, int flags):__base_socket(domain,type,flags){}
  //::connect(int fd, const struct sockaddr *addr, socklen_t len)
  int UDPsocket::connect(std::string_view address, uint16_t port){
  if(domain == AF_INET){
    sockaddr_in addr{
      .sin_family = AF_INET,
      .sin_port = htons(port)
    };
    if(!inet_pton(AF_INET,address.data(),&addr.sin_addr)){
      return -1;
    }
    return ::connect(_fd, (sockaddr*)&addr, sizeof(addr));
  }else if(domain == AF_INET6){
    sockaddr_in6 addr{
      .sin6_family = AF_INET6,
      .sin6_port = htons(port)
    };
    if(!inet_pton(AF_INET6,address.data(),&addr.sin6_addr)){
      return -1;
    }
    return ::connect(_fd, (sockaddr*)&addr, sizeof(addr));
  }
  return -1;
  }
  int UDPsocket::sendto(std::string_view address, uint16_t port, std::string_view data){
  //::sendto(int fd, const void *buf, size_t n, int flags, const struct sockaddr *addr, socklen_t addr_len)
  if(domain == AF_INET){
    sockaddr_in addr{
      .sin_family = AF_INET,
      .sin_port = htons(port)
    };
    if(!inet_pton(AF_INET,address.data(),&addr.sin_addr)){
      return -1;
    }
    return ::sendto(_fd,data.data(),data.size(),0,(sockaddr*)&addr,sizeof(addr));
  }else if(domain == AF_INET6){
    sockaddr_in6 addr{
      .sin6_family = AF_INET6,
      .sin6_port = htons(port)
    };
    if(!inet_pton(AF_INET6,data.data(),&addr.sin6_addr)){
      return -1;
    }
    return ::sendto(_fd,data.data(),data.size(),0,(sockaddr*)&addr,sizeof(addr));
  }
  return -1;
}
  int UDPsocket::bind(std::string_view address, uint16_t port, int max_conns){
  if(domain == AF_INET){
    sockaddr_in addr{
      .sin_family = AF_INET,
      .sin_port = htons(port),
    };
    if(!::inet_pton(AF_INET,address.data(), &addr.sin_addr)){
      std::cerr << "inet_pton() - " << sockError << "\n";
      return -1;
    }
    if(setsockopt(_fd,SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &__ENABLE, sizeof(__ENABLE))){
      std::cerr << "setsockopt() - " << sockError << "\n";
      return -1;
    }
    return ::bind(_fd, (sockaddr*)&addr, sizeof(addr));
  }else if(domain == AF_INET6){
    sockaddr_in6 addr{
      .sin6_family = AF_INET6,
      .sin6_port = htons(port)
    };
    if(!inet_pton(AF_INET6, address.data(), &addr.sin6_addr)){
      return -1;
    }
    if(setsockopt(_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &__ENABLE, sizeof(__ENABLE))){
      return -1;
    } 
    return ::bind(_fd,(sockaddr*)&addr,sizeof(addr));
  }
  return -1;
}
}
