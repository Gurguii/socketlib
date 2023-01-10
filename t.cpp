#include <iostream>
#include <thread>
#include <vector>

#include "prototype.hh"

using str = std::string;
using str_view = std::string_view;

int main()
{
    gsocket::tcp_socket t0; // blocking TCP socket
    gsocket::tcp_socket t1(s_protocol::noblock); // nonblocking TCP socket

    gsocket::udp_socket u0; // blocking UDP socket
    gsocket::udp_socket u1(s_protocol::noblock); // nonblocking UDP socket

    gsocket::socket s0(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); // nonblocking TCP socket
    gsocket::socket s1(AF_INET, SOCK_STREAM, 0); // blocking TCP socket

    gsocket::socket s2(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0); // nonblocking TCP socket
    gsocket::socket s3(AF_INET, SOCK_DGRAM, 0); // blocking UDP socket
}