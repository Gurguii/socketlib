#include "../core/socket_wrapper.hpp"
namespace gsocket {
int __socket_operations::close() { return ::close(fd); }
} // namespace gsocket
