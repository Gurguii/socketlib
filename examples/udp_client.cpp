#include "prototype.hh"

using str = std::string;

int main()
{
    gsocket::udp_client u("127.0.0.1", 8080);
    printf("connected to %s : %i", u.getsockname());
    for(const int &i : {0,0,0})
    {
        u.send("hellooouu\n");
    }
    u.close();
}