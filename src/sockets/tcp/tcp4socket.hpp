#pragma once
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../../core/__base_socket.hpp"
#include "tcpsocket.hpp"

namespace gsocket
{
  struct tcp4socket : public TCPsocket 
  {
protected:
  tcp4socket(int fd);
public:
  tcp4socket(BEHAVIOUR behaviour = BEHAVIOUR::block);

  tcp4socket accept();
  tcp4socket accept(addr4 &address);
  tcp4socket accept(addr &address);

  // NOT IMPLEMENTED
  tcp4socket(std::string_view addr, uint16_t port, ACTION action = ACTION::none, BEHAVIOUR behaviour = BEHAVIOUR::block);
};
}
