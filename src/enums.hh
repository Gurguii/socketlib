#ifndef gsocket_enums
#define gsocket_enums

#pragma once
#include <cstdint>

enum class Type : uint8_t {
  tcp = 1,
#define tcp Type::tcp
  udp = 2
#define udp Type::udp
};

enum class Domain : uint8_t {
  ipv4 = 2,
#define ipv4 Domain::ipv4
  ipv6 = 10
#define ipv6 Domain::ipv6
};

enum class Behaviour : uint8_t {
  noblock = 0,
#define noblock Behaviour::noblock
  block = 1
#define block Behaviour::block
};
#endif
