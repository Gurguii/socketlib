#ifndef TCP_SERVER
#define TCP_SERVER
#include "../src/socket_wrapper.hh"
namespace gsocket
{
    class tcpServer : public __sw{
        private:
        inline static int total;
        ui8 __status;
        public:
        tcpServer(str_view addr, ui16 port);
        tcpServer(ui16 port);
        int fail();
    };
}
#endif