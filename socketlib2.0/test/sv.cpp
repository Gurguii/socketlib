#include <iostream>
#include "../tcp_classes/tcp_server.cpp"
int main(){
	gsocket::tcpServer sv(8080);
	if(sv.fail()){
		std::cerr << "failed to bind \n";
		return 0;
	}
	// good 2go
}