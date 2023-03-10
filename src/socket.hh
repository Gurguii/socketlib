#ifndef SOCKET
#define SOCKET
#include <gsocket/src/core/socket_wrapper>

class Socket : public gsocket::__sw{
    public:
    Socket(Domain d, Type t, Behaviour b);
    Socket(uint8_t dom, uint8_t typ, uint8_t b);
    Socket(uint8_t fd);
};
#endif