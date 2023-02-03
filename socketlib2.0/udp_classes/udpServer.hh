#ifndef UDP_SERVER
#define UDP_SERVER
#include "../src/socket_wrapper.cpp"

class udpServer : public gsocket::__sw
{
    private:
    public:
    udpServer(std::string_view addr, uint16_t port, Behaviour b = BLOCK);
    udpServer(uint16_t port, Behaviour b = BLOCK);
};

#endif