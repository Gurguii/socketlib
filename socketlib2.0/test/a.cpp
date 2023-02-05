#include <gsocket/gsocket>
#include <gsocket/utils>
#include <gsocket/templates>
int main(){
    Socket s(inet6,tcp,BLOCK);
    auto address = gsocket::utils::getIpByIface("lo",inet6);
    s.bind(address, 8080);
    s.listen();

    auto [host,port] = gsocket::templates::getsockname(&s).value();
    std::cout << "listening ON " << host << " : " << port << "\n";
    
    std::string buffer(1024,'\x00');
    Address addr;
    for(;;){
        auto client = Socket(s.accept(addr));
        std::cout << "CONN FROM " << addr.host << " : " << addr.port << "\n";
        if(client.awaitData(buffer,4) > 0){
            std::cout << "received: " << buffer << "\n";
        };
        client.close();
        buffer.assign("");
    }
}