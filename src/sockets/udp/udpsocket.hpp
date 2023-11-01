#pragma once
#include "../../core/__base_socket.hpp"
namespace gsocket
{
  class UDPsocket : public __base_socket
  {
  protected:
    UDPsocket(int fd);
    UDPsocket(int domain, int type, int flags);
    public:
    // TODO - test
    int connect(std::string_view addr, uint16_t port);
    int sendto(std::string_view addr, uint16_t port, std::string_view data);
    int bind(std::string_view addr, uint16_t port, int max_conns = 3);
  };
}
