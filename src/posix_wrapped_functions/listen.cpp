#include "../core/socket_wrapper.hh"
namespace gsocket {
int __sw::listen(int __mConns) { return ::listen(fd, __mConns); }
} // namespace gsocket
