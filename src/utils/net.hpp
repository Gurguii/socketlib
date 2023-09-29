#pragma once
#include <ifaddrs.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include "../enums.hpp"

namespace gsocket
{
/**
 * @brief Used by getaddrinfo(). Contains preferences (hints) in order to retrieve
 *        a socket with desired characteristics.
 *        E.g `struct socketPreferences{ flags = 0, domain = tcp, type = ipv4, protocol = 0 }`
 *        - This would look for a tcp/ipv4 socket.
 */
struct socketPreferences {
  int flags;
  Domain domain;
  Type type;
  int protocol;
};

/**
 * @brief Struct that wraps addrinfo struct from POSIX to ensure we call freeaddrinfo()
 *        to free memory when the object goes out of scope.
 */
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
/**
 * @brief Returns a std::string representing the IP of the given interface.
 *        E.g getIpByIface("eth0",ipv4) -> 192.168.1.20
 * @param ifa The interface name.
 * @param t The domain (IPv4 or IPv6).
 * @return The IP address as a std::string.
 */
std::string getIpByIface(std::string_view ifa, Domain&& t);

/**
 * @brief Returns an int representing the given interface's names.
 *        E.g getIdByIp("192.168.1.20") -> 2
 * @param addr The IP address.
 * @return The interface ID as an int.
 */
int getIdByIp(std::string_view addr);

/**
 * @brief Retrieves information about the given domain and stores it in an `addressInfo` instance.
 * @param domain The domain name.
 * @param service The service name or port number.
 * @param hints The socket preferences.
 * @return An `addressInfo` instance containing information about the domain.
 */
addressInfo getaddrinfo(std::string_view domain, std::string_view service, socketPreferences& hints);

/**
 * @brief Performs the opposite of `getaddrinfo()`.
 * @param addr The address information.
 * @return A std::pair&lt;std::string, std::string&gt; with both domain and service names.
 */
std::pair<std::string, std::string> getnameinfo(addressInfo& addr);

}
