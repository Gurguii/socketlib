#include <gsocket/udp_classes/udpServer>
int main(){
    udpServer sv("::1",8080,inet6);
    msgFrom info;
    for(;;){
        sv.awaitDataFrom(info);
        std::cout << "FROM " << info.addr.host << " : " << info.addr.port << "\n" << info.msg << "\n";
    }
}