#include <memory>

#include "prototype.hh"

using str = std::string;

int main()
{
    /*  s_preferences struct which holds 
        data related with the kind of socket 
        want to connect to, in this case
        an ipv4 tcp socket */
    s_preferences s{
        .family = INET,    
        .type = TCP,         
        .protocol = 0,      
        .flags = AI_CANONNAME
    };
    /* get an addrinfo* struct which can be used in connect() */
    auto a = gsocket::getaddrinfo("www.gurgui.com", "8080", &s); // CHANGE THIS
    /* create a tcp_socket to connect to retrieved socket */
    gsocket::tcp_socket cl;
    /* try to connect */// Returns std::pair containing socket's ip and port
    if (cl.connect(a))
    {
        printf("couldn't connect to target\n");
        return 0;
    };
    /* if sucessfully connected, send a GET request */
    printf("[+] connected to %s : %i\n", cl.getpeername());
    cl.send("GET /secret.txt HTTP/1.1\r\nHost:192.168.1.49:80\r\n\r\n");
    /* wait a response and store it in data */
    str data = cl.recv();
    /* print response */
    printf("\nResponse:\n\n%s\n", &data[0]);
    /* end connection */
    cl.close();
}