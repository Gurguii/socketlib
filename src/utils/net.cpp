#include "net.hpp"

namespace gsocket
{

int getIpByIface(NetworkInterface *iface, std::string_view ifa) {
  ifaddrs *addrs = nullptr;
  if(getifaddrs(&addrs)){
    return -1;
  };
  for (auto a = addrs; a != nullptr; a = a->ifa_next) {
    if (strcmp(ifa.data(), a->ifa_name)) {
      continue;
    }
    // found desired interface, populate struct NetworkInterface
    if(a->ifa_addr->sa_family == AF_INET){
      if(inet_ntop(a->ifa_addr->sa_family,
                  &((reinterpret_cast<sockaddr_in *>(a->ifa_addr))->sin_addr),
                  &(iface->ip4)[0], INET_ADDRSTRLEN) == nullptr){
        return -1;
      };
    }else if(a->ifa_addr->sa_family == AF_INET6){
      if(inet_ntop(a->ifa_addr->sa_family,
                  &((reinterpret_cast<sockaddr_in6 *>(a->ifa_addr))->sin6_addr),
                  &(iface->ip6)[0], INET6_ADDRSTRLEN) == nullptr){
        return -1;
      };
    }
  }
  freeifaddrs(addrs);
  // everything OK
  return 0;
}

int getIdByIp(std::string_view addr) {
  ifaddrs *addrs = nullptr;
  getifaddrs(&addrs);
  std::string ad(INET6_ADDRSTRLEN, '\x00');
  for (auto a = addrs; a != nullptr; a = a->ifa_next) {
    if (a->ifa_addr->sa_family == AF_INET) {
      inet_ntop(AF_INET,
                &((reinterpret_cast<sockaddr_in *>(a->ifa_addr))->sin_addr),
                &ad[0], INET_ADDRSTRLEN);
    } else if (a->ifa_addr->sa_family == AF_INET6) {
      inet_ntop(AF_INET6,
                &((reinterpret_cast<sockaddr_in6 *>(a->ifa_addr))->sin6_addr),
                &ad[0], INET6_ADDRSTRLEN);
    }
    if (!std::strcmp(addr.data(), ad.c_str())) {
      return if_nametoindex(a->ifa_name);
    }
  }
  return -1;
}

// TODO - FIX NOT WORKING WHEN HINTS GIVEN, ELSE IT WORKS AND RETURNS EVERY RESULT
[[nodiscard("ignoring return value")]] int dns_lookup(DNSlookupResult *buffer, std::string_view domain_name_or_ip, std::string_view service,
                        SocketPreferences *hints) {
  addrinfo *addrs = nullptr;
  addrinfo *h = nullptr;
  if(hints != nullptr)
  {
    printf("hints is not null\n");
   // h->ai_flags = static_cast<int>(hints->flags);
   // h->ai_family = static_cast<int>(hints->domain);                 
   // h->ai_socktype = static_cast<int>(hints->type);                       
    h->ai_flags = 0;
    h->ai_family = AF_UNSPEC;
    h->ai_socktype = SOCK_STREAM;
    h->ai_protocol = hints->protocol;                       
    h->ai_addrlen = 0;                        
    h->ai_addr = nullptr;                     
    h->ai_canonname = nullptr;                
    h->ai_next = nullptr;              
  };
  printf("about to getaddrinfo()\n");
  if (::getaddrinfo(domain_name_or_ip.data(), service.data(), h, &addrs)) {
    freeaddrinfo(addrs);
    return -1;
  };
  std::vector<SocketInfo> results;
  for(auto iter = addrs; iter != nullptr ; iter = iter->ai_next)
  {
    SocketInfo _host{
      .type = static_cast<SOCKET_TYPE>(iter->ai_socktype),
      .domain = static_cast<SOCKET_DOMAIN>(iter->ai_family),
    };
    results.push_back(std::move(_host));
  }
  if(::getnameinfo(addrs->ai_addr,addrs->ai_addrlen, &buffer->host[0],buffer->host.size(),&buffer->service[0],buffer->service.size(),0)){
    freeaddrinfo(addrs);
    return -1;
  }
  buffer->port = ntohs(((*(uint16_t*)&addrs->ai_addr->sa_data)));
  buffer->results = std::move(results);
  buffer->addrs = addrs;
  return 0;
}
std::future<int> dns_lookup_async(DNSlookupResult *buffer, std::string_view domain_name_or_ip, std::string_view service,SocketPreferences *hints)
{
  return std::async(std::launch::async,dns_lookup,buffer,domain_name_or_ip,service,hints);
}

[[nodiscard("std::pair returned and not captured")]] std::pair<std::string, std::string> getnameinfo(DNSlookupResult res) {
  std::string _host(46, '\x00');
  std::string _serv(46, '\x00');
  if (::getnameinfo(new sockaddr(),2, &_host[0], 46,
                    &_serv[0], 46, 0)) {
    return {};
  }
  return {_host, _serv};
}

}
