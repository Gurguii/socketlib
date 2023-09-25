#include "../core/socket_wrapper.hh"
namespace gsocket {
int __sw::close() { return ::close(fd); }
} // namespace gsocket
