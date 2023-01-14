#include "prototype.hh"

using str = std::string;

int main()
{
    gsocket::udp_server sv("127.0.0.1", 8080);
    printf("listening on %s : %i\n", sv.getsockname());
    str data;
    while((data = sv.read()).size())
    {
        printf("received: %s\n", &data[0]);
    }
}