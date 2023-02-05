#include <gsocket/gsocket>
#include <gsocket/utils>
#include <gsocket/templates>
int main(){
    Socket s(inet6,tcp,BLOCK);
    auto addr = gsocket::utils::getIpByIface("lo",inet6);
    std::cout << addr << "\n";
    s.bind(addr, 8080);
    s.listen();

    auto [host,port] = gsocket::templates::getsockname(&s).value();
    std::cout << "listening ON " << host << " : " << port << "\n";

    std::string data(1024,'\x00');
    for(;;){
        auto client = Socket(s.accept());
        if(client.awaitData(data,4) > 0){
            std::cout << "received: " << data << "\n";
        };
        client.close();
    }
}