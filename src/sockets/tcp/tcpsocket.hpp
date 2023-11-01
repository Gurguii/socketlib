#pragma once
#include <arpa/inet.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <future>
#include <thread>
#include <any>

#include "../../utils/net.hpp"
#include "../../core/__base_socket.hpp"

namespace gsocket
{
class TCPsocket : public __base_socket
{
  protected:
    TCPsocket(int fd);
  public:
    /* methods */
    TCPsocket accept();
    int connect(std::string_view host, uint16_t port);
    int bind(std::string_view ipaddr, uint16_t port, int max_conns = 3);
    // IMPLEMENT
  };
}
