#include "../core/socket_wrapper.hpp"
namespace gsocket {
int __socket_operations::send(std::string_view d) { return ::send(fd, d.data(), d.size(), 0); }
int __socket_operations::send(std::string_view d, int &b) { return ::send(fd, d.data(), b, 0); };
int __socket_operations::send(std::string_view d, int &&b) { return ::send(fd, d.data(), b, 0); };
int __socket_operations::send(const char *&d) { return ::send(fd, d, strlen(d), 0); }
int __socket_operations::send(const char *&&d) { return ::send(fd, d, strlen(d), 0); }
int __socket_operations::send(std::string &d) { return ::send(fd, d.c_str(), d.size(), 0); }
template <typename... Args> int send(Args &&...args) {
  return send(std::forward<Args>(args)...);
}

} // namespace gsocket
