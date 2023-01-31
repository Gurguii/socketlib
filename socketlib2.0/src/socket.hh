#ifndef SOCKET
#define SOCKET
#include "socket_wrapper.hh"
namespace gsocket{
    class Socket : public __sw{
        public:
        Socket(Domain d, Type t, Behaviour b);
        Socket(int dom, int typ, int beh);
        Socket(int fd);
        Socket acceptConnection();
    };
}
#endif