#ifndef TCP_CLIENT
#define TCP_CLIENT
#include "../src/socket_wrapper.hh"
struct Client{
    std::string host;
    uint16_t port;
    Domain dom;
    Behaviour beh;
};
namespace gsocket
{
    class tcpClient : public __sw{
        private:
        inline static int total{0};
        int __status = 0;
        public:
        tcpClient(std::string_view host, uint16_t port, Domain d = inet, Behaviour b = BLOCK);
        tcpClient(Client client);
        int fail();
    };
}
#endif