#include "socket_wrapper.hh"

namespace gsocket {
/* __sw C'tors */
__sw::__sw(Domain d, Type t, Behaviour b)
    : domain(static_cast<int>(d)),
      type(b == BLOCK ? static_cast<int>(t)
                      : static_cast<int>(t) | SOCK_NONBLOCK),
      fd(socket(domain, type, 0)) {}
__sw::__sw(ui8 d, ui8 t, ui8 p) : domain(d), type(t), fd(socket(d, t, p)) {}
__sw::__sw(ui8 fd) : fd(fd) {}
__sw::~__sw() { ::close(fd); }
} // namespace gsocket
