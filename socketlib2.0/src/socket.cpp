#include "socket.hh"
namespace gsocket{
    Socket::Socket(Domain d, Type t, Behaviour b):__sw(d,t,b){};
    Socket::Socket(int d, int t, int b):__sw(d,t,b){};
    Socket::Socket(int fd):__sw(fd){};
    Socket Socket::acceptConnection(){
        return accept();
    }
}