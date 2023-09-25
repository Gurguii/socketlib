#include "../core/socket_wrapper.hh"
namespace gsocket {
int __sw::accept() {
  sockaddr addr;
  socklen_t addrlen = sizeof(addr);
  return ::accept(fd, &addr, &addrlen);
}

template <typename AddrStruct> int __sw::accept(AddrStruct &a) {
  sockaddr addr;
  socklen_t addrlen = sizeof(addr);
  uint8_t n = ::accept(fd, &addr, &addrlen);
  if (addr.sa_family == AF_INET) {
    a.host = inet_ntoa(reinterpret_cast<sockaddr_in *>(&addr)->sin_addr);
    a.port = htons(reinterpret_cast<sockaddr_in *>(&addr)->sin_port);
  } else if (addr.sa_family == AF_INET6) {
    inet_ntop(AF_INET6, &reinterpret_cast<sockaddr_in6 *>(&addr)->sin6_addr,
              a.host.data(), INET6_ADDRSTRLEN);
    a.port = htons(reinterpret_cast<sockaddr_in6 *>(&addr)->sin6_port);
  }
  return n;
}
} // namespace gsocket
