#include "../__base_socket.hpp"

namespace gsocket
{
int __base_socket::recv(char *buffer, size_t size){
  return ::recv(_fd,buffer,size,0);
 }
 int __base_socket::recv(std::string &buffer){
  return ::recv(_fd,&buffer[0],buffer.size(),0);
 }
}
