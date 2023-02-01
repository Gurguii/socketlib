#include <iostream>
#include "src/enums.hh"
#include "src/socket_wrapper.cpp"
#include "src/socket.cpp"
#include "src/utils.cpp"

using namespace gsocket;
int main(){
	Socket sv(inet,udp,BLOCK);
	sv.bind("127.0.0.1", 8080);
	msgFrom data;
	for(;;){
		sv.recvfrom(data);
		std::cout << "FROM " << data.host << ":" << data.port << "\nreceived: " << data.msg << "\n";
	}
}