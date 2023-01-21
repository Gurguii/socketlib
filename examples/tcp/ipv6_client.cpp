#include "prototype.hh"

int main()
{
    gsocket::tcp_client cl(gsocket::getIpByIface("lo", Ipv6), 8080, Ipv6);
    cl.send("hello from ipv6 socket\n");
    cl.close();
}