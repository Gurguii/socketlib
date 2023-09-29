#pragma once
#include <optional>
#include "../../core/socket_wrapper.hpp"
namespace gsocket
{
  class tcpServer : public __socket_operations
  {
  private:
    Status _status;
  protected:
    static std::optional<std::string_view> _lasterror;
  public:
    tcpServer(std::string_view host, uint16_t port);
    Status status();
    static std::string_view const getLastError();
  };
}
