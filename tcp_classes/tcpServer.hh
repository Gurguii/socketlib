#ifndef TCP_SERVER
#define TCP_SERVER
#include <gsocket/src/core/socket_wrapper>
#include <gsocket/socket>

class tcpServer : public gsocket::__sw{
    private:
    inline static int __total{0};
    uint8_t __status;
    public:
    tcpServer(std::string_view addr,uint16_t port,Domain domain = inet,Behaviour b = BLOCK,int maxconns = 3);
    tcpServer(uint16_t port,Domain domain = inet,Behaviour b = BLOCK,int maxconns = 3);
    int fail();
    int total();
    Socket acceptConnection();
    Socket acceptConnection(msgFrom &storage);
};

#endif