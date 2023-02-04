#ifndef TCP_SERVER_CPP
#define TCP_SERVER_CPP

#include "tcpServer.hh"

tcpServer::tcpServer(std::string_view addr,uint16_t port,Behaviour b,int __maxconns)
:__sw(AF_INET, (b == BLOCK ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)),0)
{
    __status = (bind(addr,port) | listen(__maxconns));
}
tcpServer::tcpServer(uint16_t port,Behaviour b,int __maxconns)
:__sw(AF_INET, (b == BLOCK ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)),0)
{
    __status = (bind(port) | listen(__maxconns));
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
#endif