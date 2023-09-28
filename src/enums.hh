#pragma once
#include <cstdint>
#include <sys/socket.h>

enum class Type : uint8_t {
  tcp = 1,
#define tcp_sock Type::tcp
  udp = 2
#define udp_sock Type::udp
};

enum class Domain : uint8_t {
  local = 0,
#define local_sock Domain::local
  ipv4 = 2,
#define ipv4 Domain::ipv4
#define ipv4_sock Domain::ipv4
  ipv6 = 10
#define ipv6 Domain::ipv6
#define ipv6_sock Domain::ipv6
};

enum class Behaviour : uint8_t {
  noblock = 0,
#define noblock_sock Behaviour::noblock
  block = 1
#define block_sock Behaviour::block
};
