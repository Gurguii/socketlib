#include "socket.hh"
namespace gsocket
{
    Socket::Socket(Domain d, Type t, Behaviour b):__sw(d,t,b){};
    Socket::Socket(ui8 d, ui8 t, ui8 p):__sw(d,t,p){};
    Socket::Socket(ui8 fd):__sw(fd){};
}