#include "prototype.hh"

int main()
{
    try{
        auto iface_ip = gsocket::getIpByIface("eth0", inet6);
        // note that 'eth0' is required in this case cause we're using an Ipv6 sock
        // if this was ipv4 we could do gsocket::tcp_client cl4(iface_ip, 80)
        gsocket::tcp_client cl6(iface_ip, 80, inet6, BLOCK, "eth0");
        if(cl6.failure()){
            fprintf(stderr, "couldn't connect\n");
            return 0;
        }
        std::string msg(1024, '\x00');
        for(;;){
            printf("msg: ");
            getline(std::cin, msg);
            cl6.send(msg);
        }
    }catch(gsocket::CustomExceptions &err){
        fprintf(stderr, "%s\n", err.what().c_str());
        return 0;
    };

}   