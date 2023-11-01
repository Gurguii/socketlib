#include "fd.hpp"
int get_available_bytes2(gsocket::__base_socket *sockptr){
  int available = 0;
  ioctl(sockptr->_fd,FIONREAD,&available);
  return available;
};
