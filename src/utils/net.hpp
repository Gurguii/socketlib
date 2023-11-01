#pragma once
// C++ headers
#include <string>
#include <cstring>
#include <cstdint>
#include <future>

// C headers
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/types.h>
#include <netinet/in.h>

// gsocket headers
#include "../enums.hpp"
#include "../structs.hpp"

#define netError gai_strerror(errno)
namespace gsocket
{
int getIpByIface(NetworkInterface *iface, std::string_view ifa);

int getIdByIp(std::string_view addr);

int dns_lookup(DNSlookupResult *buffer, std::string_view domain_name_or_ip, std::string_view service, SocketPreferences* hints = nullptr);
std::future<int> dns_lookup_async(DNSlookupResult *buffer, std::string_view domain_name_or_ip, std::string_view service, SocketPreferences* hints = nullptr);

// TODO - ADD getnameinfo() wrapper
}
