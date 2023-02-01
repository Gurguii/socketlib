#include "tcp_server.hh"
namespace gsocket
{
    tcpServer::tcpServer(str_view addr, ui16 port)
    :__sw(AF_INET, SOCK_STREAM, 0){
        __status = __sw::bind(addr, port);
        ++total;
    }
    tcpServer::tcpServer(ui16 port)
    :__sw(AF_INET, SOCK_STREAM, 0){
        __status = __sw::bind(port);
        ++total;
    }
    int tcpServer::fail(){
        return __status;
    }
}