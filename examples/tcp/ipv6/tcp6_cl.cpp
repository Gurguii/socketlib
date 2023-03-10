#include <gsocket/tcp_classes/tcpClient>
int main(){
    tcpClient cl("::1",8080,inet6);
    if(cl.fail()){
        std::cerr << "can't connect - " << sockError << "\n";
        return 0;
    }
    cl.send("hello from tcpClient");
    cl.close();
}