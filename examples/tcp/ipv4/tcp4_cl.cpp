#include <gsocket/tcpClient>
int main(){
    tcpClient cl("127.0.0.1", 8080);
    if(cl.fail()){
        std::cerr << "can't connect - " << sockError << "\n";
        return 0;
    }
    cl.send("hello from tcpClient");
    cl.close();
}