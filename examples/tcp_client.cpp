#include "../prototype.hh"

int main()
{
    std::string data;
    std::string msg = "hello from gsocket::tcp_client\n";

    gsocket::tcp_client cl("127.0.0.1", 8080); // CHANGE
    cl.reset();
    if(cl.connected())
    {
        cl.send(msg);
        cl.close();
    }
}