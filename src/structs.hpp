#pragma once
#include <string>
#include <cstdint>

/**
 * @defgroup structs Structures
 * This group contains the following structures:
 *
 * - Addr4
 * - Addr6
 * - msgFrom
 *
 * @{
 */

/**
 * @struct Addr4
 * Structure used to store an IPv4 IP and port.
 *
 * @members
 *   host: The IPv4 IP address, as a string.
 *   port: The port number, as an unsigned 16-bit integer.
 */
struct Addr4 {
  std::string host = std::string(16, '\x00');
  uint16_t port;
};

/**
 * @struct Addr6
 * Structure used to store an IPv4/IPv6 IP and port.
 *
 * @members
 *   host: The IPv4/IPv6 IP address, as a string.
 *   port: The port number, as an unsigned 16-bit integer.
 */
struct Addr6 {
  std::string host = std::string(46, '\x00');
  uint16_t port;
};

/**
 * @struct msgFrom
 * Structure used in some functions (such as readfrom()) as a parameter/return value.
 * Contains `struct Address` with host-port and std::string which holds the received msg.
 *
 * @members
 *   addr: The address of the sender, as an Addr6 structure.
 *   msg: The message received, as a string.
 */
struct msgFrom {
  Addr6 addr;
  std::string msg;
};

/** @} */ // end of structs group

