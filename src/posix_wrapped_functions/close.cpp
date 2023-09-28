#include "../core/socket_wrapper.hh"
namespace gsocket {
int __socket_operations::close() { return ::close(fd); }
} // namespace gsocket
