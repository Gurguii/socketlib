#ifndef ENUMS
#define ENUMS
#ifndef AF_INET
#include <sys/socket.h>
#endif
enum class Type : short unsigned{
    tcp = 1,
    #define tcp Type::tcp
    udp = 2
    #define udp Type::udp
};
enum class Domain : short unsigned{
    inet = 2,
    #define inet Domain::inet
    inet6 = 10,
    #define inet6 Domain::inet6
};
enum class Behaviour : short unsigned{
    noblock = 0,
    #define NOBLOCK Behaviour::noblock
    block = 1
    #define BLOCK Behaviour::block
};
#endif