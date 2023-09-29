#include "tcpServer.hpp"

namespace gsocket
{
  std::optional<std::string_view> tcpServer::_lasterror;
  tcpServer::tcpServer(std::string_view host, uint16_t port)
  :__socket_operations(Domain::ipv4,Type::tcp,Behaviour::block)
  {
    // make this human readable :)
    _status = ( bind(host,port) | listen() ) 
              ? Status::unbinded
              : Status::binded;
    if(_status == Status::unbinded){_lasterror = sockError;}
  }
  Status tcpServer::status(){
    return _status;
  }
  std::string_view const tcpServer::getLastError(){
    return _lasterror.value_or("no errors yet");
  }
}
