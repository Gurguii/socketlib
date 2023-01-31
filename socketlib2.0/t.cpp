#include "src/socket_wrapper.cpp"
#include "src/socket.cpp"
int main(int argc, const char *args[]){
	socketPreferences fav{
		.domain = inet,
		.type = tcp,
		.protocol = 0,
		.flags = AI_CANONNAME
	};
	gsocket::utils::getaddrinfo("hola soy ", "gurgui");
	//auto res = gsocket::utils::getaddrinfo("www.gurgui.com", "80", fav);
	//auto h = res.get();
	gsocket::Socket client(inet, tcp, BLOCK);
	//if(client.connect(h)){
	//	std::cerr << "couldn't connect to target\n";
	//};
	client.send("GET /secret.txt HTTP/1.1\r\nHost:192.168.1.99\r\n\r\n");
	std::string data;
	if(client.awaitData(data, 3) == -2){
		std::cerr << "response timeout\n";
		client.close();
		return 0;
	};
	std::cout << "response:\n" << data;
	client.close();
}
