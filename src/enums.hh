#ifndef gsocket_enums
#define gsocket_enums
#include <cstdint>
enum class Type : uint8_t {
  tcp = 1,
#define tcp Type::tcp
  udp = 2
#define udp Type::udp
};

enum class Domain : uint8_t {
  inet = 2,
#define inet Domain::inet
  inet6 = 10
#define inet6 Domain::inet6
};

enum class Behaviour : uint8_t {
  noblock = 0,
#define NOBLOCK Behaviour::noblock
  block = 1
#define BLOCK Behaviour::block
};
#endif
