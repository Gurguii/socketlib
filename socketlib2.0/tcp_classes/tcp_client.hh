#ifndef TCP_CLIENT
#define TCP_CLIENT
#include "../src/socket_wrapper.hh"
namespace gsocket
{
    class tcpClient : public __sw{
        private:
        inline static int total{0};
        int __status = 0;
        public:
        tcpClient(str_view host, uint16_t port, Behaviour b = BLOCK);
        int fail();
    };
}
#endif