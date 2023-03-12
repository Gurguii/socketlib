#include <gsocket/tcp_classes/tcpClient>
int main(){
    tcpClient cl("127.0.0.1", 8080);
    if(cl.fail()){
        std::cerr << "can't connect - " << sockError << "\n";
        return 0;
    }
    std::string msg;
    for(;;){
        printf("msg: ");
        getline(std::cin, msg);
        cl.send(msg);
    }
}
