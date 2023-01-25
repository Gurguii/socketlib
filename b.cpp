#include "prototype.hh"

int main(){
    gsocket::tcp_server sv("127.0.0.1", 8080, inet, 3, BLOCK);
    printf("%s : %i\n", sv.getsockname());
    auto cl = sv.accept_connection();
    printf("from %s : %i\nreceived: %s\n", sv.getpeername(), cl.recv().c_str());
    cl.close();
}