#include "socket_wrapper.hh"

namespace gsocket {
/* __sw C'tors */
__sw::__sw(Domain d, Type t, Behaviour b)
    : domain(static_cast<int>(d)),
      type(b == block ? static_cast<int>(t)
                      : static_cast<int>(t) | SOCK_NONBLOCK),
      fd(socket(domain, type, 0)) {}
__sw::__sw(uint8_t d, uint8_t t, uint8_t p) : domain(d), type(t), fd(socket(d, t, p)) {}
__sw::__sw(uint8_t fd) : fd(fd) {}
__sw::~__sw() { ::close(fd); }
} // namespace gsocket
