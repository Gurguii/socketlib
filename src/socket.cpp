#ifndef GSOCKET_CPP
#define GSOCKET_CPP
#include "socket.hh"
Socket::Socket(Domain d, Type t, Behaviour b) : __sw(d, t, b){};
Socket::Socket(uint8_t d, uint8_t t, uint8_t p) : __sw(d, t, p){};
Socket::Socket(uint8_t fd) : __sw(fd){};
#endif
