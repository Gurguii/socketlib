#pragma once
#include "../../core/__base_socket.hpp"
#include "udpsocket.hpp"

namespace gsocket
{
  class udp4socket : public UDPsocket
  {
  public:
    udp4socket(BEHAVIOUR behaviour = BEHAVIOUR::noblock);
  };
}
