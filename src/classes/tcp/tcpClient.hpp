#pragma once
#include "../../core/socket_wrapper.hpp"

namespace gsocket
{
  class tcpClient : public __socket_operations
  {
  private:
    Status _status;
  protected:
    static std::string_view _lasterror;
  public:
    // constructor
    tcpClient(std::string_view host, uint16_t port); 
    // members
    Status status();
    std::string_view const getLastError();
  };
}
