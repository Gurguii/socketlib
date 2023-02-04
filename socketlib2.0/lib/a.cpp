#include <iostream>
#include "../src/socket_wrapper.hh"
#include "../src/socket.hh"
#include "../tcp_classes/tcp_server.hh"
#include "../prototype/test.hh"

int main(){
	tcpServer sv("127.0.0.1", 8080);
	if(sv.fail()){
		std::cerr << "can't bind/listen - " << sockError << "\n";
		return 0;
	}
	auto [host,port] = proto::getsockname(&sv).value();
	std::cout << "listening on " << host << " : " << port << "\n";
	msgFrom clientData;
	for(;;){
		auto client = sv.acceptConnection(clientData);
		std::cout << "CONN FROM " << clientData.addr.host << " : " << clientData.addr.port << "\n";
		for(;;){
			if(proto::awaitData(&client,&clientData.msg,5) <= 0){
				std::cerr << "smth bad happened - "  << sockError << "\n";
				client.close();
				clientData.msg.assign("");
				break;
			}
			std::cout << "received: " << clientData.msg << "\n";
			clientData.msg.assign("");
		}	
	}
}
