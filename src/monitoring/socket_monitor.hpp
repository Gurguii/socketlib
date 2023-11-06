#pragma once

#include <unordered_map>
#include <sys/epoll.h>
#include <signal.h>
#include "../core/__base_socket.hpp"

/* POSIX enum EPOLL_EVENTS wrapper */
enum class SOCKET_EVENTS : uint32_t
{
  NONE = 0x00,
  READABLE = 0x001,
  OOB_DATA_AVAILABLE = 0x002,
  WRITABLE = 0x004,
  ERROR = 0x008,
  CONN_CLOSED = 0x010
};
using EVENTS = SOCKET_EVENTS;

uint32_t operator|(EVENTS lev, EVENTS rev);
bool operator&(EVENTS lev, EVENTS rev);

// TODO - implement this by using wrappers to epoll, etc. that inherit from these
namespace gsocket::monitoring
{
  class SocketMonitor
  {
  private:
    int _epoll;
    uint64_t managed_descriptors = 0;
  public:
    std::vector<epoll_event> available_events{};

    SocketMonitor();
    // TODO - implement
    void add(int fd, EVENTS events);
    void add(gsocket::__base_socket *socket, EVENTS events);
    void remove(int fd);
    void remove(gsocket::__base_socket *socket);
    void clear_events();
    int wait_events(int max_events = 10, int timeout = -1);
    EVENTS get_events(gsocket::__base_socket *socket);
  }; 
}
