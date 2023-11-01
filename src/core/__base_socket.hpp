#pragma once

// C++ headers
#include <iostream>
#include <string_view>
#include <cstdint>
#include <variant>
#include <optional>

// C headers
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

// gsocket headers
#include "../structs.hpp"
#include "../enums.hpp"

#define sockError strerror(errno)

#ifndef DEFAULT_AWAIT_TIMEOUT
#define DEFAULT_AWAIT_TIMEOUT -1 
#endif

// ALIASES
using TYPE = gsocket::SOCKET_TYPE;
using BEHAVIOUR = gsocket::SOCKET_BEHAVIOUR;
using DOMAIN = gsocket::SOCKET_DOMAIN;
using ACTION = gsocket::SOCKET_ACTION;

// CONSTANT EXPRESSIONS
constexpr int __sockopt_enable{1};
constexpr int __sockopt_disable{0};

namespace gsocket
{
  class __base_socket
  {
  protected:
    // Constructors
    __base_socket(uint8_t fd);
    __base_socket(uint8_t domain, uint8_t type, uint8_t flags);

    // Destructor
    ~__base_socket();
  public:
    uint8_t const _fd;
    uint8_t domain, type, behaviour;

    // Common methods for all socket types
    // @return -1 == error -> sockError/netError to retrieve error 
    int send(std::string_view data);
    int send(char *buffer, size_t size);
    int send(FILE *file); 
    int send(File &file); 

    int recv(std::string &buffer);
    int recv(char *buffer, size_t size);
    
    int close();

    int duplicate();
    int duplicate(int newfd);
    int duplicate(int newfd, int flags);
    
    int await(std::string &buffer, int timeout = -1, __sigset_t *_signals = nullptr);
    int await(void *buff, uint64_t nbytes, int timeout = -1, __sigset_t *_signals = nullptr);

    int get_address(addr &buffer);

    // TODO - implement
    int get_peer_address(addr &buffer);
  };
}
