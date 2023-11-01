#include <gsocket/gsocket.hpp>
using namespace gsocket;
int main()
{
  tcp4socket sock;
  if(sock.bind("127.0.0.1", 9001)){
    std::cerr << sockError << "\n";
    return -1;
  }
  addr server_addr;
  sock.get_address(server_addr);
  printf("listening on %s:%d\n",std::get<addr4>(server_addr).host(),std::get<addr4>(server_addr).port);
  char buff[1024], msg[1024];
  memset(buff,0,sizeof(buff));
  memset(msg,0,sizeof(buff));
  addr4 clientaddr;
  monitoring::SocketMonitor watcher;
  uint64_t rbytes = 0;
  for(;;){
    printf("[+] accepting connections...\n");
    tcp4socket client = sock.accept(clientaddr);
    printf("connection from %s:%d\n",clientaddr.host(),clientaddr.port);
    watcher.add(&client,SOCKET_EVENTS::READABLE);
    for(;;){
      printf("[+] waiting for msg...\n");
      SOCKET_EVENTS events = (SOCKET_EVENTS)watcher.wait_events();
      if(events & SOCKET_EVENTS::READABLE){
        rbytes = client.recv(buff,sizeof(buff));
        if(rbytes <= 0){
          break;
        }
        int cx = snprintf(msg,sizeof(msg),"[%lu bytes] received - %s\n",rbytes,buff);
        write(1,msg,sizeof(msg));
        memset(buff,0,rbytes);
        memset(msg,0,cx > 0 ? cx : sizeof(msg));
      }
    }
    printf("[info] connection closed\n");
    client.close();
  }
}
