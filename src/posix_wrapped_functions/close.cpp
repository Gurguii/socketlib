#ifndef __SOCK_CLOSE
#define __SOCK_CLOSE
#include "../core/socket_wrapper.hh"
namespace gsocket {
int __sw::close() { return ::close(fd); }
} // namespace gsocket
#endif
