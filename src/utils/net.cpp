#include "net.hpp"
#include <thread>
namespace gsocket
{

std::string getIpByIface(std::string_view ifa, Domain &&t = g_ipv4) {
  ifaddrs *addrs = nullptr;
  getifaddrs(&addrs);
  std::string iface((t == g_ipv4 ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN), '\x00');
  for (auto a = addrs; a != nullptr; a = a->ifa_next) {
    if (a->ifa_addr->sa_family == static_cast<int>(t)) {
      if (strcmp(ifa.data(), a->ifa_name)) {
        continue;
      }
      if (static_cast<int>(t) == AF_INET) {
        inet_ntop(a->ifa_addr->sa_family,
                  &((reinterpret_cast<sockaddr_in *>(a->ifa_addr))->sin_addr),
                  &iface[0], INET_ADDRSTRLEN);
      } else if (static_cast<int>(t) == AF_INET6) {
        inet_ntop(a->ifa_addr->sa_family,
                  &((reinterpret_cast<sockaddr_in6 *>(a->ifa_addr))->sin6_addr),
                  &iface[0], INET6_ADDRSTRLEN);
      }
      break;
    }
  }
  freeifaddrs(addrs);
  return iface;
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

addressInfo getaddrinfo(std::string_view domain, std::string_view service,
                        socketPreferences &hints) {
  addrinfo *addrs = nullptr;
  addrinfo h{hints.flags,
             static_cast<int>(hints.domain),
             static_cast<int>(hints.type),
             hints.protocol,
             0,
             nullptr,
             nullptr,
             nullptr};
  if (::getaddrinfo(domain.data(), service.data(), &h, &addrs)) {
    return nullptr;
  };
  return addressInfo(addrs);
}
std::pair<std::string, std::string> getnameinfo(addressInfo &addr) {
  std::string _host(46, '\x00');
  std::string _serv(46, '\x00');
  if (::getnameinfo(addr.get()->ai_addr, addr.get()->ai_addrlen, &_host[0], 46,
                    &_serv[0], 46, 0)) {
    return {};
  }
  return {_host, _serv};
}
template <typename T> std::pair<T, T> getsocketpair(Type t, Behaviour b) {
  std::pair<int, int> fds;
  ::socketpair(AF_LOCAL,
               (b == g_block ? static_cast<int>(t)
                           : (static_cast<int>(t) | SOCK_NONBLOCK)),
               0, &fds.first);
  return fds;
}
}
