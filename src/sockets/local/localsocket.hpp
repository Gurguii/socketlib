#pragma once
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include "../../core/__base_socket.hpp"

namespace gsocket
{
class localsocket : public sockaddr_un, public __base_socket
{
  private:
    uint8_t is_server = 0; // 1 if constructor called with SOCKET_ACTION::bind
    int connect();
    int bind();

  protected:
    localsocket(int fd);

  public:
    ~localsocket();// pathname local socket
    localsocket(const char *path, ACTION action, TYPE type = TYPE::stream, BEHAVIOUR behaviour = BEHAVIOUR::block);
    localsocket accept();   
    // TODO - implement abstract/unnamed
};
}
