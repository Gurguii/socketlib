#ifndef __SOCK_SENDTO
#define __SOCK_SENDTO
#include "../core/socket_wrapper.hh"
namespace gsocket {
int __sw::sendto(str_view host, ui16 port, str_view msg) {
  if (domain == AF_INET) {
    sockaddr_in addr{
        .sin_family = domain,
        .sin_port = htons(port),
    };
    if (inet_pton(domain, msg.data(), &addr.sin_addr)) {
      return -2;
    }
    return ::sendto(fd, msg.data(), msg.size(), 0,
                    reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
  } else if (domain == AF_INET6) {
    sockaddr_in6 addr{.sin6_family{AF_INET6}, .sin6_port{htons(port)}};
    if (inet_pton(domain, msg.data(), &addr.sin6_addr)) {
      return -2;
    }
    return ::sendto(fd, msg.data(), msg.size(), 0,
                    reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
  }
}
template <typename... Args> int sendto(Args &&...args) {
  return sendto(std::forward<Args>(args)...);
};
} // namespace gsocket

#endif
