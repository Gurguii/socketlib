#include "socket_wrapper.cpp"
#include "socket.cpp"
#include <vector>
int main(){
    gsocket::Socket s(inet, tcp, BLOCK);
    if(s.bind("127.0.0.1", 8000)){
        std::cerr << "error binding - " << sockError << "\n";
        return 0;
    }
    const auto [host, port] = s.getsockname();
    std::cout << "listening on " << host << " : " << port << "\n";
    s.listen();
    msgFrom clientData;
    for(;;){
        auto newClient = gsocket::Socket(s.accept());
        //std::cout << "CONN FROM " << clientData.host << " : " << clientData.port << "\n";
        for(;;){
            int n = newClient.awaitDataFrom(clientData, 4);
            if(n == -1 || n == 0 || n == -2){
                std::cout << (n == 0 ? "closed\n" : "bad\n");
                newClient.close();
                break;
            }
            std::cout << "FROM " << clientData.host << " " << clientData.port << "\ndata: " << clientData.msg << "\n";
            clientData.msg.assign("");
        }
    }
    //auto newClient = s.accept();
}