#include "../core/socket_wrapper.hh"
namespace gsocket {
int __sw::send(std::string_view d) { return ::send(fd, d.data(), d.size(), 0); }
int __sw::send(std::string_view d, int &b) { return ::send(fd, d.data(), b, 0); };
int __sw::send(std::string_view d, int &&b) { return ::send(fd, d.data(), b, 0); };
int __sw::send(const char *&d) { return ::send(fd, d, strlen(d), 0); }
int __sw::send(const char *&&d) { return ::send(fd, d, strlen(d), 0); }
int __sw::send(std::string &d) { return ::send(fd, d.c_str(), d.size(), 0); }
template <typename... Args> int send(Args &&...args) {
  return send(std::forward<Args>(args)...);
}

} // namespace gsocket
