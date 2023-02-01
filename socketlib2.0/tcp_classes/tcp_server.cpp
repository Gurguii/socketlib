#include "tcp_server.hh"
namespace gsocket
{
    tcpServer::tcpServer(str_view addr,ui16 port,Behaviour b)
    :__sw(AF_INET, (b == BLOCK ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)),0)
    {
        __status = (bind(addr,port) | listen());
    }
    tcpServer::tcpServer(ui16 port,Behaviour b)
    :__sw(AF_INET, (b == BLOCK ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)),0)
    {
        __status = (bind(port) | listen());
    }
    int tcpServer::fail(){
        return __status;
    }
    int tcpServer::total(){
        return __total;
    }
    Socket tcpServer::acceptConnection(){
        return accept();
    }
    Socket tcpServer::acceptConnection(msgFrom &m){
        return accept(m.addr);
    }
}