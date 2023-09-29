#pragma once
#include "../socket.hpp"

namespace gsocket::socket_generator
{
  constexpr auto tcp4_socket = [](){
    return Socket(g_ipv4,g_tcp,g_block); 
  };
  
  constexpr auto tcp6_socket = [](){
    return Socket(g_ipv6,g_tcp,g_block);
  };
  
  constexpr auto udp4_socket = [](){
    return Socket(g_ipv4,g_udp,g_block);
  };
  
  constexpr auto udp6_socket = [](){
    return Socket(g_ipv6,g_udp,g_block);
  };
  
  constexpr auto local_socket = [](){
    return Socket(g_local,g_tcp,g_block);
  };
}
