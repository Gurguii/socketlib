#pragma once
#include <arpa/inet.h>
#include <cstring>
#include <ifaddrs.h>
#include <iostream>
#include <net/if.h>
#include <netdb.h>
#include <optional>
#include <string>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../utils.hh"

#define sockError strerror(errno)
#define __IO_BUFFSIZE 4096

namespace gsocket {
using ui16 = uint16_t;
using ui8 = uint8_t;
using str = std::string;
using str_view = std::string_view;
class __sw {
protected:
  __sw(Domain d, Type t, Behaviour b);
  __sw(ui8 domain, ui8 type, ui8 protocol);
  __sw(ui8 fd);
  ~__sw();

public:
  ui8 domain, type, fd;
  /* GET FILE DESCRIPTOR */
  const int getFD() { return fd; }
  /* CLOSE */
  int close();
  /* CONNECT */
  int connect(const char *&host, int &port);
  int connect(const char *&&host, int &&port);
  int connect(addrinfo *const addr);
  int connect(addressInfo *addr);
  int connect(sockaddr *addr, socklen_t &addrlen);
  int connect(std::string_view &host, uint16_t &port);
  template <typename... Args> int connect(Args &&...args);
  /* SEND DATA */
  int send(str_view data);
  int send(str_view data, int &bytes);
  int send(str_view data, int &&bytes);
  int send(const char *&data);
  int send(const char *&&data);
  int send(std::string &data);
  template <typename... Args> int send(Args &&...args);
  int sendto(str_view host, uint16_t port, str_view msg);
  template <typename... Args> int sendto(Args &&...args);





  /* RECV DATA */
  /* recv all available data from socket
   * @return std::string with the contents */
  std::string recv();
  /* recv `bytes` bytes from socket 
   * @returns std::string with the contents */
  std::string recv(int bytes);
  /* populates `buffer` with `bytes_to_read` bytes 
   * @returns number of bytes received or -1 for errors */
  int recv(char *buffer, size_t bytes_to_read);
  /* mainly used with udp sockets
   * takes `struct msgFrom` as parameter.
   * It will populate the `struct msgFrom` with the
   * incoming host - port - message */
  int recvfrom(msgFrom &data);
  



  /* AWAIT DATA */
  template <typename T> int awaitData(T &buffer, int timeout = -1);
  int awaitDataFrom(msgFrom &__sockHostData, int timeout);
  /* BIND */
  int bind(str_view addr, uint16_t port);
  int bind(uint16_t port);
  template <typename... Args> int bind(Args &&...args);
  /* LISTEN */
  int listen(int maxConns = 3);
  /* ACCEPT */
  int accept();
  template <typename AddrStruct> int accept(AddrStruct &a);
  /* RETRIEVE SOCKET DATA */
  Address getsockname();
  Address getpeername();
};
} // namespace gsocket
