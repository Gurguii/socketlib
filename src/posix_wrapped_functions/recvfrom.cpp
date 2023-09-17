#ifndef __SOCK_RECVFROM
#define __SOCK_RECVFROM
#include "../core/socket_wrapper.hh"
namespace gsocket {
int __sw::recvfrom(msgFrom &data) {
  if (domain == AF_INET) {
    sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    int &&n = ::recvfrom(fd, data.msg.data(), data.msg.size(), 0,
                         reinterpret_cast<sockaddr *>(&addr), &addrlen);
    data.addr = {inet_ntoa(addr.sin_addr), htons(addr.sin_port)};
    return n;
  } else if (domain == AF_INET6) {
    sockaddr_in6 addr;
    socklen_t addrlen = sizeof(addr);
    int &&n = ::recvfrom(fd, data.msg.data(), data.msg.size(), 0,
                         reinterpret_cast<sockaddr *>(&addr), &addrlen);
    std::string ad(46, '\x00');
    inet_ntop(AF_INET6, &addr.sin6_addr, &ad[0], INET6_ADDRSTRLEN);
    data.addr.host = ad;
    data.addr.port = htons(addr.sin6_port);
    return n;
  }
}
} // namespace gsocket

#endif
