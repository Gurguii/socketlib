#ifndef UDP_SERVER__HH
#define UDP_SERVER__HH
#include <gsocket/src/core/socket_wrapper>

class udpServer : public gsocket::__sw
{
    private:
    public:
    udpServer(std::string_view addr, uint16_t port,Domain domain = inet, Behaviour b = BLOCK);
    udpServer(uint16_t port,Domain domain = inet, Behaviour b = BLOCK);
};

#endif