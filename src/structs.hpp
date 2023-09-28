#pragma once
#include <string>
#include <cstdint>
/**
 * @brief Stores IPv4 IP and port.
 */
struct Addr4 {
  std::string host = std::string(16, '\x00');
  uint16_t port;
};

/**
 * @brief Simple structure used to store an IPv4/IPv6 IP and a port.
 */
struct Addr6 {
  std::string host = std::string(46, '\x00');
  uint16_t port;
};

/**
 * @brief Used in some functions (such as readfrom()) as a parameter/return value.
 *        Contains `struct Address` with host-port and std::string which holds the received msg.
 */
struct msgFrom {
  Addr6 addr;
  std::string msg;
};


