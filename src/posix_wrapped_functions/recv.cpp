#include "../core/socket_wrapper.hh"
#include <sys/socket.h>
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

int __sw::recv(char *buffer, size_t bytes_to_read) {
  return (::recv(fd, buffer, bytes_to_read, 0));
}
} // namespace gsocket
