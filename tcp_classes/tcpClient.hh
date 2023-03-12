#ifndef TCP_CLIENT
#define TCP_CLIENT
#include <gsocket/src/core/socket_wrapper>
#include <gsocket/socket>

class tcpClient : public gsocket::__sw{
    private:
    inline static int total{0};
    int __status = 0;
    public:
    tcpClient(std::string_view host, uint16_t port,Domain domain = inet, Behaviour b = BLOCK);
    int fail();
};

#endif