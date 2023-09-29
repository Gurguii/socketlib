#include "tcpClient.hpp"

namespace gsocket
{
  std::string_view tcpClient::_lasterror;

  tcpClient::tcpClient(std::string_view host, uint16_t port)
  :__socket_operations(Domain::ipv4, Type::tcp, Behaviour::block)
  {
    if(connect(host,port)){
      // couldn't connect
      _status = Status::disconnected;
      _lasterror = sockError;
    }else{
      _status = Status::connected;
    }
  }
  Status tcpClient::status(){
    return _status;
  }
  std::string_view const tcpClient::getLastError(){
    return _lasterror; 
  }
}
