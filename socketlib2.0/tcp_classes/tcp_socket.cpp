#include "tcp_socket.hh"

namespace gsocket
{
    tcpSocket::tcpSocket(Behaviour b)
    :__sw(AF_INET, (b == BLOCK ? AF_INET : (AF_INET | SOCK_NONBLOCK)), 0)
    {}
}