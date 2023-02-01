#ifndef TCP_SERVER
#define TCP_SERVER
#include "../src/socket_wrapper.hh"
namespace gsocket
{
    class tcpServer : public __sw{
        private:
        inline static int __total{0};
        ui8 __status;
        public:
        tcpServer(str_view addr,ui16 port,Behaviour b = BLOCK);
        tcpServer(ui16 port,Behaviour b = BLOCK);
        int fail();
        int total();
        Socket acceptConnection();
        Socket acceptConnection(msgFrom &storage);
    };
}
#endif