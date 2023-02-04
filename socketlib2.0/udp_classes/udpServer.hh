#ifndef UDP_SERVER__HH
#define UDP_SERVER__HH
#include "../src/socket_wrapper.hh"

class udpServer : public gsocket::__sw
{
    private:
    public:
    udpServer(std::string_view addr, uint16_t port, Behaviour b = BLOCK);
    udpServer(uint16_t port, Behaviour b = BLOCK);
};

#endif