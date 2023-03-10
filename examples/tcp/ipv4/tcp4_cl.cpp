#include <gsocket/tcp_classes/tcpClient>
#include <gsocket/src/posix_wrapped_functions/connect>
#include <gsocket/src/posix_wrapped_functions/send>
int main(){
    tcpClient cl("127.0.0.1", 8080);
    if(cl.fail()){
        std::cerr << "can't connect - " << sockError << "\n";
        return 0;
    }
    cl.send("hello from tcpClient");
    cl.close();
}