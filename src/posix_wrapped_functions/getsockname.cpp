#include "../core/socket_wrapper.hh"
namespace gsocket {
Address __sw::getsockname() {
  if (domain == AF_INET) {
    sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    ::getsockname(fd, reinterpret_cast<sockaddr *>(&addr), &addrlen);
    return Address{inet_ntoa(addr.sin_addr), htons(addr.sin_port)};
  } else if (domain == AF_INET6) {
    std::string ip(INET6_ADDRSTRLEN, '\x00');
    sockaddr_in6 addr;
    socklen_t adlen = sizeof(addr);
    ::getsockname(fd, reinterpret_cast<sockaddr *>(&addr), &adlen);
    inet_ntop(AF_INET6, &addr.sin6_addr, ip.data(), INET6_ADDRSTRLEN);
    return Address{ip, htons(addr.sin6_port)};
  }
  return Address{};
}
} // namespace gsocket
