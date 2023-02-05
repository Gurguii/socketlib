#include <gsocket/gsocket>
int main(){
    Socket s(inet6,tcp,BLOCK);
    s.connect("::1",8080);
    s.send("hellooowii\n");
    s.close();
}