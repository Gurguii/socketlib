#include <iostream>

#include "../prototype.hh"

using str = std::string;

int main()
{
    str message = "hi from fd0\n";

    auto [fd0, fd1] = gsocket::getsocketpair(UDP); // argument s_type must be TCP/UDP (or s_types::<tcp/udp>)

    int bytes_sent = fd0.send(message);
    std::cout << "fd0 sent " << bytes_sent << " bytes of data\n";

    str data_received = fd1.recv();
    std::cout << "fd1 received: " << data_received << "\n";
}