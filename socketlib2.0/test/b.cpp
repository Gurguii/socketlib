#include <gsocket/gsocket>
#include <gsocket/utils>
#include <gsocket/templates>
int main(){
    Socket s(inet,tcp,BLOCK);
    socketPreferences hints{
        .flags{AI_CANONNAME},
        .domain{inet6},
        .type{tcp},
        .protocol{0}
    };
    auto gurguinet = gsocket::utils::getaddrinfo("www.google.com","443", hints);
    if (gurguinet.get() == nullptr){
        std::cout << "null\n";
    }
    //s.connect(&gurguinet);
    auto [addr,srv] = gsocket::utils::getnameinfo(gurguinet);
    std::cout << addr << " : " << srv << "\n";
    //s.send("GET /secret.txt HTTP/1.1\r\nHost: gsocket\r\n\r\n");
    //std::string buff(1024,'\x00');
    //gsocket::templates::awaitData(&s,&buff,4);
    //std::cout << "received: " << buff << "\n";
    //s.close();
}