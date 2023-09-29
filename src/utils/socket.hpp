#pragma once
#include "../enums.hpp"
#include <utility>

namespace gsocket{

/**
 * @brief Gets a pair of connected sockets.
 * @param type The socket type.
 * @param behaviour The socket behaviour.
 * @return A std::pair with a pair of AF_UNIX / AF_TIPC connected sockets.
 */
std::pair<int, int> getsocketpair(Type type, Behaviour behaviour);


}
