#ifndef TCP_SOCKET
#define TCP_SOCKET
#include "../src/socket_wrapper.hh"

namespace gsocket
{
    class tcpSocket : public __sw{
        public:
        tcpSocket(Behaviour b = BLOCK);
    };
}
#endif
