#include "../core/socket_wrapper.hpp"
constexpr int opt = 1;
namespace gsocket {
int __socket_operations::bind(std::string_view addr, uint16_t p) {
  if (domain == AF_INET) {
    sockaddr_in ad{.sin_family = domain, .sin_port = htons(p)};
    return (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                       sizeof(opt)) |
            !inet_pton(domain, addr.data(), &ad.sin_addr)) |
           ::bind(fd, reinterpret_cast<sockaddr *>(&ad), sizeof(ad));
  } else if (domain == AF_INET6) {
    sockaddr_in6 ad{.sin6_family = AF_INET6,
                    .sin6_port = htons(p),
                    .sin6_scope_id =
                        static_cast<uint32_t>(gsocket::getIdByIp(addr))};
    return (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                       sizeof(opt)) |
            !inet_pton(domain, addr.data(), &ad.sin6_addr)) |
           ::bind(fd, reinterpret_cast<sockaddr *>(&ad), sizeof(ad));
  } else {
    return -1;
  }
}
int __socket_operations::bind(uint16_t p) {
  if (domain == AF_INET) {
    sockaddr_in addr{
        .sin_family = AF_INET, .sin_port = htons(p), .sin_addr{INADDR_ANY}};
    return (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                       sizeof(opt)) |
            ::bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)));
  } else if (domain == AF_INET6) {
    sockaddr_in6 addr{
        .sin6_family = AF_INET6, .sin6_port = htons(p), .sin6_addr{INADDR_ANY}};
    return (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                       sizeof(opt)) |
            ::bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)));
  } else {
    return -1;
  }
}
template <typename... Args> int bind(Args &&...args) {
  return bind(std::forward<Args>(args)...);
}
} // namespace gsocket
