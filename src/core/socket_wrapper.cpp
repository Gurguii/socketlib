#include "socket_wrapper.hpp"

namespace gsocket {

/* GET FILE DESCRIPTOR */
int const __socket_operations::getFD(){
  return fd;
}
/* CONSTRUCTORS __socket_operations */
__socket_operations::__socket_operations(Domain d, Type t, Behaviour b)
    : domain(static_cast<int>(d)),
      type(b == Behaviour::block ? static_cast<int>(t)
                      : static_cast<int>(t) | SOCK_NONBLOCK),
      fd(socket(domain, type, 0)) {}

__socket_operations::__socket_operations(uint8_t d, uint8_t t, uint8_t p) : domain(d), type(t), fd(socket(d, t, p)) {}

__socket_operations::__socket_operations(uint8_t fd) : fd(fd) {}

/* DESTRUCTOR __socket_operations */
__socket_operations::~__socket_operations() { ::close(fd); }
} // namespace gsocket
