#include "__base_socket.hpp"


namespace gsocket
{
 
 __base_socket::__base_socket(uint8_t fd):_fd(fd){}
  
 __base_socket::__base_socket(uint8_t domain, uint8_t type, uint8_t flags)
 :domain(domain),_fd(socket(domain,type,flags)){}

 __base_socket::~__base_socket(){
  close();
 }
 int __base_socket::close(){
  return ::close(_fd);
 } 

/* main gsocket structure: `addr`
 * which holds:
 * uint32_t ipv4
 * uint32_t[4] ipv6
 *
 * and method get() to retrieve 
 * human readable addr-port*/
} // namespace gsocket
