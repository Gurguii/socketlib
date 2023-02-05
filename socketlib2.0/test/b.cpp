#include <gsocket/gsocket>
int main(){
    Socket s(inet,tcp,BLOCK);
    s.connect("::1",8080);
    s.send("hellowiiii");
    s.close();
}