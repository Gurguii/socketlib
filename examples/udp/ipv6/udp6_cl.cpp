#include <gsocket/udp_classes/udpClient>
#include <gsocket/socket>
#include <gsocket/misc/utils>
int main(){
    udpClient cl("::1",8080,inet6);
    std::string buff;
    for(;;){
        std::cout << "msg: ";
        getline(std::cin,buff);
        if(buff == "close" | buff == "exit"){
            return 0;
        }
        cl.send(buff);
    }
}