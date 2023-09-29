#include "../core/socket_wrapper.hpp"
#include <cstdint>
constexpr uint16_t __TIMEOUT_MULTIPLIER = 1000;
namespace gsocket {
int __socket_operations::awaitData(std::string &buff, int timeout) {
  auto fdpol = pollfd{.fd = fd, .events = POLLIN};
  int r = poll(&fdpol, 1, (timeout > 0 ? timeout * __TIMEOUT_MULTIPLIER : -1));
  if (r > 0 && (fdpol.revents & POLLIN)) {
    int b;
    ioctl(fd, FIONREAD, &b);
    if (buff.size() < b) {
      buff.resize(buff.size() + b);
    }
    return ::recv(fd, buff.data(), buff.size(), 0);
  }
  return -2;
}
int __socket_operations::awaitDataFrom(msgFrom &__data, int timeout = -1) {
  auto _s_poll = pollfd{.fd = fd, .events = POLLIN};
  int event, avBytes, rStatus;
  event = poll(&_s_poll, 1,
               (timeout == -1 ? -1 : (timeout * __TIMEOUT_MULTIPLIER)));
  if (event > 0 && (_s_poll.revents & POLLIN)) {
    // data available
    ioctl(fd, FIONREAD, &avBytes);
    if (__data.msg.size() < avBytes) {
      __data.msg.resize(avBytes);
    }
    sockaddr addr;
    socklen_t &&addrlen = sizeof(addr);
    int &&n = ::recvfrom(fd, __data.msg.data(), avBytes, 0, &addr, &addrlen);
    if (n != -1) {
      __data.addr.host =
          inet_ntoa(reinterpret_cast<sockaddr_in *>(&addr)->sin_addr);
      __data.addr.port =
          htons(reinterpret_cast<sockaddr_in *>(&addr)->sin_port);
    }
    return n;
  }
  return -2;
}
} // namespace gsocket
