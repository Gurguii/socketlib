#ifndef UTILS
#define UTILS
#include "enums.hh"
#include <netdb.h>
#include <string>
#include <sys/ioctl.h>

/* stores ipv4 ip and port */
struct Addr4 {
  std::string host = std::string(16, '\x00');
  uint16_t port;
};

/* Simple structure used to store 
 * an ipv4/ipv6 ip and a port */
struct Address {
  std::string host = std::string(46, '\x00');
  uint16_t port;
};

/* Used in some functions (such as readfrom()) as a parameter/return value
 * contains `struct Address` with host-port
 * and std::string which holds the received msg
 */
struct msgFrom {
  Address addr;
  std::string msg;
};

/* Used by getaddrinfo()
 * contains preferences (hints) in order to retrieve
 * socket with desired characteristics 
 * E.g `struct socketPreferences{
 *  flags = 0,
 *  domain = tcp,
 *  type = inet,
 *  protocol = 0
 * }` - This would look for a tcp/ipv4 socket
 * */
struct socketPreferences {
  int flags;
  Domain domain;
  Type type;
  int protocol;
};

/* struct that wraps addrinfo() struct from POSIX to
 * ensure we call freeaddrinfo() to free memory when 
 * object goes out of scope */
struct addressInfo {
private:
  addrinfo* head;

public:
  addressInfo(addrinfo* addrs) : head(addrs) {}
  addrinfo* get() { return head; };
  ~addressInfo() {
    /* free addrinfo memory when object goes out of scope */
    freeaddrinfo(head);
  }
};

namespace gsocket::utils {
using str = std::string;
using str_view = std::string_view;

/* returns `std::string` representing the 
  * ip of the given interface  E.g getIpByIface("eth0") -> 192.168.1.20 */
str getIpByIface(str_view ifa, Domain &&t);

/* returns `int` representing given interface's names - E.g getIdByIp("192.168.1.20") -> 2 */
int getIdByIp(str_view addr);

/* returns `addressInfo` instance which contains information about given domain in the
 * addrinfo property, which can be retrieved with the get() function and contains :
 *  Input flags.  
 *  Protocol family for socket.  
 *  Socket type.  
 *  Protocol for socket.  
 *  Length of socket address.  
 *  Socket address for socket.  
 *  Canonical name for service location.  
 *  Pointer to next in list.  */
addressInfo getaddrinfo(str_view domain, str_view service,
                        socketPreferences &hints);

std::pair<std::string, std::string> getnameinfo(addressInfo &addr);

/* returns `std::pair` with a pair of AF_UNIX / AF_TIPC connected sockets 
 * */ 
std::pair<int,int> getsocketpair(Type type, Behaviour behaviour);


/* returns available bytes (amount of buffered bytes to be read) in a file_descriptor */
constexpr auto availableBytes = [](int fd) {
  int ab;
  ioctl(fd, FIONREAD, &ab);
  return ab;
};
} // namespace gsocket::utils

/* returns `std::pair<std::string,std::string>` with the common name and service name of given addressInfo struct 
 * */
std::pair<std::string, std::string> getnameinfo(addressInfo &addr);
#endif
