#include <gsocket/tcp_classes/tcpServer>
constexpr int TIMEOUT{4};
int main(){
    tcpServer sv("127.0.0.1", 8080);
    if(sv.fail()){
        std::cerr << "can't bind - " << sockError << "\n";
        return 0;
    }
    const auto [host,port] = sv.getsockname();
    std::cout << "[+] listening on " << host << " " << port << "\n";
    std::cout << "[+] connection timeout: " << TIMEOUT << "s\n";
    msgFrom info;
    int r;
    for(;;){
        auto newClient = sv.acceptConnection();
        std::cout << "CONN FROM " << host << " : " << port << "\n";
        for(;;){
            info.msg.assign("");
            r = newClient.awaitData(info.msg,TIMEOUT);
            if(r > 0){
                std::cout << "received: " << info.msg << "\n";
                continue;
            }
            switch(r){
                case 0:
                std::cout << "connection closed\n";
                break;
                case -1:
                std::cerr << "error - " << sockError << "\n";
                break;
                case -2:
                std::cout << "timeout\n";
                break;
                default:
                ;
            }
            newClient.close();
            break;
        }
    }
}