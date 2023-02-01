#include "tcp_client.hh"
namespace gsocket
{
    tcpClient::tcpClient(str_view h,ui16 p,Behaviour b)
    :__sw(AF_INET, (b == BLOCK ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)),0)
    {
        __status = connect(h,p);
    }
    int tcpClient::fail(){
        return __status;
    }
}