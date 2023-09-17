#ifndef __SOCK_RECV
#define __SOCK_RECV
#include "../core/socket_wrapper.hh"
namespace gsocket {
std::string __sw::recv() {
  std::string buffer(__IO_BUFFSIZE, '\x00');
  int n = ::recv(fd, buffer.data(), __IO_BUFFSIZE, 0);
  if (n == -1 || n == 0) {
    return "";
  }
  if (n == __IO_BUFFSIZE) {
    int avbytes;
    ioctl(fd, FIONREAD, &avbytes);
    buffer.resize(__IO_BUFFSIZE + avbytes);
    ::recv(fd, &buffer[__IO_BUFFSIZE], avbytes, 0);
  }
  return buffer;
}
std::string __sw::recv(int bytes) {
  std::string buffer(bytes, '\x00');
  if (::recv(fd, buffer.data(), bytes, 0) == -1) {
    return "";
  };
  return buffer;
}
} // namespace gsocket

#endif
