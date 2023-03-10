#include <gsocket/udp_classes/udpClient>
int main(){
    udpClient ucl("127.0.0.1", 8080); // CHANGE
    std::string msg(1024,'\x00');
    for(;;){
        printf("msg: ");
        getline(std::cin,msg);
        if(msg == "close" || msg == "exit"){
            return 0;
        }
        printf("msg is %s\n", msg.c_str());
        ucl.send(msg);
    }
}