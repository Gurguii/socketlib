#ifndef UDP_CLIENT_HH
#define UDP_CLIENT_HH
#include "../src/socket_wrapper.hh"

class udpClient : public gsocket::__sw
{
    private:
    inline static int total{0};
    public:
    udpClient(std::string_view host,uint16_t port,Behaviour b = NOBLOCK);
    udpClient(Address &addr,Behaviour b = NOBLOCK);
};

#endif