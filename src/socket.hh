#ifndef GSOCKET_HH
#define GSOCKET_HH
#include "core/socket_wrapper.hh"
class Socket : public gsocket::__sw {
public:
  Socket(Domain d, Type t, Behaviour b);
  Socket(uint8_t dom, uint8_t typ, uint8_t b);
  Socket(uint8_t fd);
};
#endif
