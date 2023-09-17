#include "templates.hh"
namespace gsocket::templates {
template <class gsocketClass>
// CONNECT
int connect(gsocketClass *ptr, std::string_view addr, uint16_t port) {
  sockaddr_in h{h.sin_family = ptr->domain, h.sin_port = htons(port)};
  inet_pton(ptr->domain, addr.data(), &h.sin_addr);
  return ::connect(ptr->fd, reinterpret_cast<sockaddr *>(&h), sizeof(h));
}
// SEND
template <class gsocketClass>
int send(gsocketClass *ptr, std::string_view data) {
  return ::send(ptr->fd, data.data(), data.size(), 0);
}
// RECV
template <class gsocketClass, class A> int recv(gsocketClass *ptr, A *buff) {
  return ::recv(ptr->fd, buff->data(), buff->size(), 0);
}
// AWAITDATA
template <class gsocketClass, class A>
int awaitData(gsocketClass *ptr, A *buff, int timeout) {
  auto fdp = pollfd{.fd{ptr->fd}, .events{POLLIN}};
  auto a = poll(&fdp, 1, (timeout == -1 ? -1 : timeout * 1000));
  int n = 0;
  if (a > 0 && (fdp.revents & POLLIN)) {
    ioctl(ptr->fd, FIONREAD, &n);
    if (buff->size() < n) {
      buff->resize(buff->size() + n);
    }
    return ::recv(ptr->fd, buff->data(), buff->size(), 0);
  }
  return -2;
}

// BIND
template <class gsocketClass>
int bind(gsocketClass *ptr, std::string_view h, uint16_t port) {
  sockaddr_in addr{.sin_family{ptr->domain}, .sin_port{htons(port)}};
  int opt{1};
  return (setsockopt(ptr->fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                     sizeof(opt)) |
          (inet_pton(ptr->domain, h.data(), &addr.sin_addr) <= 0) |
          ::bind(ptr->fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)));
}

// LISTEN
template <class gsocketClass> int listen(gsocketClass *ptr, int __maxconns) {
  return ::listen(ptr->fd, __maxconns);
}

// ACCEPTCONNECTION
template <class gsocketClass>
Socket acceptConnection(gsocketClass *ptr, Address *buff) {
  sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  int fd = ::accept(ptr->fd, reinterpret_cast<sockaddr *>(&addr), &addrlen);
  if (buff == nullptr) {
    return Socket(fd);
  }
  buff->host = inet_ntoa(addr.sin_addr);
  buff->port = htons(addr.sin_port);
  return Socket(fd);
}

// GETSOCKNAME
template <class gsocketClass>
std::optional<Address> getsockname(gsocketClass *ptr, Address *buff) {
  sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  int s = ::getsockname(ptr->fd, reinterpret_cast<sockaddr *>(&addr), &addrlen);
  if (buff == nullptr) {
    return std::optional<Address>(
        {inet_ntoa(addr.sin_addr), htons(addr.sin_port)});
  }
  buff->host = inet_ntoa(addr.sin_addr);
  buff->port = htons(addr.sin_port);
  return {};
}
} // namespace gsocket::templates
