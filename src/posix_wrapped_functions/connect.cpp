#include "../core/socket_wrapper.hpp"
namespace gsocket {
int __socket_operations::connect(const char *&h, int &p) {
  if (domain == AF_INET) {
    // IPv4
    sockaddr_in addr{
      .sin_family = AF_INET, 
      .sin_port = htons(p)
    };
    return (!inet_pton(domain, h, &addr.sin_addr) |
            ::connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)));
  } else if (domain == AF_INET6) {
    // IPv6
    sockaddr_in6 addr{
      .sin6_family = AF_INET6, 
      .sin6_port = htons(p)
    };
    return (!inet_pton(domain, h, &addr.sin6_addr) |
            ::connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)));
  } else {
    return -1;
  }
}
int __socket_operations::connect(const char *&&h, int &&p) {
  if (domain == AF_INET) {
    // IPv4
    sockaddr_in addr{
      .sin_family = AF_INET, 
      .sin_port = htons(p)
    };
    return (!inet_pton(domain, h, &addr.sin_addr) |
            ::connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)));
  } else if (domain == AF_INET6) {
    // IPv6
    sockaddr_in6 addr{
      .sin6_family = AF_INET6, 
      .sin6_port = htons(p)
    };
    return (!inet_pton(domain, h, &addr.sin6_addr) |
            ::connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)));
  } else {
    return -1;
  }
}
int __socket_operations::connect(addrinfo *const addr) {
  return ::connect(fd, addr->ai_addr, addr->ai_addrlen);
}
int __socket_operations::connect(addressInfo *addr) {
  return ::connect(fd, addr->get()->ai_addr, addr->get()->ai_addrlen);
}
int __socket_operations::connect(sockaddr *addr, socklen_t &addrlen) {
  return ::connect(fd, addr, addrlen);
}
int __socket_operations::connect(std::string_view &host, uint16_t &port) {
  if (domain == AF_INET) {
    sockaddr_in addr{.sin_family = AF_INET, .sin_port = htons(port)};
    return (!inet_pton(AF_INET, host.data(), &addr.sin_addr) |
            ::connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)));
  } else {
    return -1;
  }
}
/* Perfect forwarding */
template <typename... Args> int connect(Args &&...args) {
  return connect(std::forward<Args>(args)...);
}
} // namespace gsocket
