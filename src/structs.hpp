#pragma once
#include <arpa/inet.h>
#include <string>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>
#include <variant>
#include "enums.hpp"
#include "utils/dnsflags.hpp"



namespace gsocket
{

struct File{
  const char *path;
};

struct addr4
{
  uint32_t addr;
  uint16_t port;
  [[nodiscard("returning std::string not being used")]] char* host(){
    return inet_ntoa(in_addr{addr});
  }
};

struct addr6
{
  in6_addr addr;
  uint16_t port;
  // TODO - implement
  [[nodiscard("ignoring returned std::string")]] std::string host(){
    return nullptr;
  }
};

//struct addr{
//  private:
//  std::variant<gsocket::addr4,gsocket::addr6> __addr;
//  public:
//  char* host;
//  uint16_t port;
//}
using addr = std::variant<gsocket::addr4,gsocket::addr6>;

struct NetworkInterface 
{
  std::string ip4 = std::string(INET_ADDRSTRLEN,'\x00');
  std::string ip6 = std::string(INET6_ADDRSTRLEN, '\x00');
  // TODO - implement this
  uint32_t _ip4;
  uint8_t _ip6[16];
};
struct SocketPreferences {
  DNS_FLAGS flags;
  SOCKET_DOMAIN domain;
  SOCKET_TYPE type;
  int protocol;
};
struct SocketInfo
{
  SOCKET_TYPE type;
  SOCKET_DOMAIN domain;
};
//dnsLookupResult
struct DNSlookupResult
{
public:
  std::string host = std::string(46,'\x00');
  std::string service = std::string(10,'\x00');
  uint16_t port;

  addrinfo *addrs = nullptr;
  std::vector<SocketInfo> results;

  ~DNSlookupResult(){
    /* free resources */
    if(addrs != nullptr){
      ::freeaddrinfo(addrs);
    }
  };
}; 
}
