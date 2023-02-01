#ifndef SOCKET
#define SOCKET
#include "socket_wrapper.hh"
namespace gsocket{
    class Socket : public __sw{
        public:
        Socket(Domain d, Type t, Behaviour b);
        Socket(ui8 dom, ui8 typ, ui8 b);
        Socket(ui8 fd);
    };
}
#endif