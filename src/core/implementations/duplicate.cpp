#include "../__base_socket.hpp"

namespace gsocket
{
 int __base_socket::duplicate(){
  return ::dup(_fd);
 }
 int __base_socket::duplicate(int newfd){
  return ::dup2(_fd,newfd); 
 }
 int __base_socket::duplicate(int newfd, int flags){
  return ::dup3(_fd,newfd,flags);
 }
}
