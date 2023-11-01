#pragma once
#include <cstddef>
#include <ios>
#include <netdb.h>

enum class DNS_FLAGS : uint8_t
{
  NUMERIC_HOST = 1,
  NUMERIC_SERVICE = 2, 
  NO_FQDN = 4,
  NO_NUMERIC_ADDRESSES = 8,
  UDP_SERVICE = 16
};
/* Custom operator overloads @ dnsflags.cpp */
