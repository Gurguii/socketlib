#ifndef UDP_CLIENT_HH
#define UDP_CLIENT_HH
#include <gsocket/src/core/socket_wrapper>

class udpClient : public gsocket::__sw
{
    private:
    inline static int total{0};
    public:
    udpClient(std::string_view host,uint16_t port,Domain domain = inet,Behaviour b = NOBLOCK);
    udpClient(Address &addr,Domain domain = inet,Behaviour b = NOBLOCK);
};

#endif