#include "tcp_client.hh"
namespace gsocket
{
    tcpClient::tcpClient(std::string_view host, uint16_t port, Domain d, Behaviour b)
    :__sw(AF_INET, (b == BLOCK ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)),0)
    {
        __status = connect(host, port);
        ++total;
    }
    tcpClient::tcpClient(Client client)
    :__sw(AF_INET, (client.beh == BLOCK ? SOCK_STREAM : (SOCK_STREAM | SOCK_NONBLOCK)),0)
    {
        __status = connect(client.host,client.port);
        ++total;
    }
    int tcpClient::fail(){
        return __status;
    }
    //tcpClient::tcpClient(Client cl)
}