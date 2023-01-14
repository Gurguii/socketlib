#include "prototype.hh"

using str = std::string;

int main()
{
    gsocket::udp_socket s;
    s.connect("127.0.0.1", 8080);
    s.send("1231231dsca\n");
    printf("%s : %i\n%s : %i\n", s.getsockname(), s.getpeername());
    s.send("32131ksca\n");
    s.close();
    return -1;
    //s.connect("127.0.0.1", 8080);
    for(const int &i : {0,0,0})
    {
        s.sendto("127.0.0.1", 8080, "hellouu\n");
    }
    s.close();
}