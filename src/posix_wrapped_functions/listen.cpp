#include "../core/socket_wrapper.hh"
namespace gsocket {
int __socket_operations::listen(int __mConns) { return ::listen(fd, __mConns); }
} // namespace gsocket
