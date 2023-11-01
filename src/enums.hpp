#pragma once
#include <cstdint>

/* enums.hpp
* This file contains enum classes that wrap the POSIX sockets enums
* to make it more legible adapted to the lib */
namespace gsocket
{

enum class SOCKET_DOMAIN : uint8_t 
{
  local = 1,
  ipv4 = 2,
  ipv6 = 10,
};

enum class SOCKET_TYPE : uint8_t
{
  stream = 1,
  datagram = 2,
  raw = 3
};

/* TODO : MAKE THIS PER-CALL ACTIONS, IMPLEMENT ITS USAGE IN FUNCTIONS */
enum class SOCKET_FLAGS : uint8_t
{
  noblock,
  close_on_exec
};

/* not a POSIX wrapper */
enum class SOCKET_ACTION : uint8_t
{
  connect,
  bind,
  none
};

/* not a POSIX wrapper */
enum class SOCKET_BEHAVIOUR : uint8_t
{
  block,
  noblock,
};

/* not a POSIX wrapper */
enum class SOCKET_STATE : uint8_t
{
  none = 0,
  bound,
  connected,
  disconnected,
  unbound,
};

}
