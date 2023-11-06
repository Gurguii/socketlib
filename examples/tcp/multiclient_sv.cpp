#include <gsocket/gsocket.hpp>
using namespace gsocket;
int main()
{
  const char *host = "127.0.0.1";
  uint16_t port = 9001;
  tcp4socket server;
  if(server.bind(host, port)){
    std::cerr << sockError << "\n";
    return -1;
  }
  printf("LISTENING ON %s:%d\n",host,port);
  addr4 addrbuff;
  for(;;){
    printf("ACCEPTING CONNECTIONS\n");
    tcp4socket client = server.accept(addrbuff);
    printf("CONNECTION FROM %s:%d\n",addrbuff.host(),addrbuff.port);
    if(fork() == 0){
      addr4 clientaddr = std::move(addrbuff);
      std::string client_info = std::string("[") + clientaddr.host() + std::string(":") + std::to_string(clientaddr.port) + std::string("] - ");
      char buffer[4096];
      uint64_t rbytes = 0;
      memset(buffer,0,sizeof(buffer));
      while((rbytes = client.recv(buffer,sizeof(buffer))) > 0){
        write(1,client_info.c_str(),client_info.size());
        write(1,buffer,rbytes);
        memset(buffer,0,rbytes);
      }
      printf("CLIENT %s:%d CLOSED CONNECTION\n",clientaddr.host(),clientaddr.port);
      client.close();
      memset(buffer,0,sizeof(buffer));
    }
  }
}
