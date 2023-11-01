#include "dnsflags.hpp"

DNS_FLAGS operator|(DNS_FLAGS op1,DNS_FLAGS op2){
  return static_cast<DNS_FLAGS>(static_cast<int>(op1) | static_cast<int>(op2));
}

uint8_t operator&(DNS_FLAGS op1, DNS_FLAGS op2){
  return (static_cast<uint8_t>(op1) & static_cast<uint8_t>(op2));
}
