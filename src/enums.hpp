#pragma once
#include <cstdint>
#include <sys/socket.h>

/**
 * @defgroup enums Enum types
 * This group contains all of the enum types used in the library.
 * @{
 */

/**
 * @enum Type
 * Enum type for the type of socket.
 * @values
 *   tcp: TCP socket
 *   udp: UDP socket
 */
enum class Type : uint8_t {
  tcp = 1,
#define g_tcp Type::tcp
  udp = 2
#define g_udp Type::udp
};

/**
 * @enum Domain
 * Enum type for the domain of the socket.
 * @values
 *   local: Local socket
 *   ipv4: IPv4 socket
 *   ipv6: IPv6 socket
 */
enum class Domain : uint8_t {
  local = 1,
#define g_local Domain::local
  ipv4 = 2,
#define g_ipv4 Domain::ipv4
  ipv6 = 10
#define g_ipv6 Domain::ipv6
};

/**
 * @enum Behaviour
 * Enum type for the behaviour of the socket.
 * @values
 *   noblock: Non-blocking socket
 *   block: Blocking socket
 */
enum class Behaviour : uint8_t {
  noblock = 0,
#define g_noblock Behaviour::noblock
  block = 1
#define g_block Behaviour::block
};

/**
 * @enum Status
 * Enum type for the status of the socket.
 * @values
 *   disconnected: Disconnected socket
 *   connected: Connected socket
 *   binded: Bound socket
 *   unbinded: Unbound socket
 */
enum class Status : uint8_t {
  disconnected = 0,
#define g_disconnected Status::disconnected
  connected = 1,
#define g_gconnected Status::connected
  binded = 2,
#define g_binded Status::binded
  unbinded = 3,
#define g_unbinded Status::unbinded
};

/** @} */ // end of enums group
