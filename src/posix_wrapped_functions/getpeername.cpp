#include "../core/socket_wrapper.hh"

namespace gsocket {
Addr6 __socket_operations::getpeername() {
  if (domain == AF_INET) {
    sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    ::getpeername(fd, reinterpret_cast<sockaddr *>(&addr), &addrlen);
    return {inet_ntoa(addr.sin_addr), htons(addr.sin_port)};
  } else if (domain == AF_INET6) {
    std::string ip(INET6_ADDRSTRLEN, '\x00');
    sockaddr_in6 addr;
    socklen_t addrlen = sizeof(addr);
    ::getpeername(fd, reinterpret_cast<sockaddr *>(&addr), &addrlen);
    inet_ntop(AF_INET6, &addr.sin6_addr, ip.data(), INET6_ADDRSTRLEN);
    return {ip, htons(addr.sin6_port)};
  }
  return Addr6();
}
} // namespace gsocket
