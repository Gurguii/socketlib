#ifndef __GSOCKET_TEMPLATE
#define __GSOCKET_TEMPLATE
#include "../src/core/socket_wrapper.hh"
#include "../src/socket.hh"
#include <string_view>

namespace gsocket::templates {

template <class gsocketClass>
int connect(gsocketClass *ptr, std::string_view addr, uint16_t port);

template <class gsocketClass>
int send(gsocketClass *ptr, std::string_view data);

template <class gsocketClass, class A> int recv(gsocketClass *ptr, A *buff);
/* Note: this template function does not empty the buffer, what means previous
   data might be displayed if you don't empty it somehow e.g clear() assign("")
    Example: first read -> "gurgui" || second read -> "aaa"  || buff -> aaagui
 */
template <class gsocketClass, class A>
int awaitData(gsocketClass *ptr, A *buff, int timeout = -1);

template <class gsocketClass>
int bind(gsocketClass *ptr, std::string_view h, uint16_t port);

template <class gsocketClass> int listen(gsocketClass *ptr, int __maxconns = 3);

template <class gsocketClass>
Socket acceptConnection(gsocketClass *ptr, Address *buff = nullptr);

template <class gsocketClass>
std::optional<Address> getsockname(gsocketClass *ptr, Address *buff = nullptr);

} // namespace gsocket::templates
#endif
