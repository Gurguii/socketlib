#include <gsocket/gsocket.hpp>
using namespace gsocket;
using namespace gsocket::monitoring;
static uint8_t buffer[4 * 1024 * 1024]{0};
int main() 
{
  SocketMonitor watcher;
  tcp4socket sv;
  if(sv.bind("127.0.0.1", 9001)){
    std::cerr << sockError << "\n"; return -1;
  }
  addr4 client_addr;
  uint64_t rbytes = 0;
  for(;;)
  {
    printf("[+] accepting connections\n");
    tcp4socket client = sv.accept(client_addr);  
    watcher.add(&client,EVENTS::READABLE);
    printf("[+] CONNECTION FROM %s:%d\n", client_addr.host(), client_addr.port);
    for(;;){
      int w = watcher.wait_events(4000);
      if(w == 0){
        printf("[+] client timeout\n");
        break;
      }
      for(epoll_event ev : watcher.available_events){
        if(ev.data.fd == client._fd){
          if(ev.events & (uint32_t)EVENTS::READABLE){
            int av = 0;
            get_available_bytes(client._fd,&av);
            rbytes = client.recv((char*)buffer,sizeof(buffer)); 
            if(rbytes <= 0){
              break;
            }
            printf("[client] - %s\n", buffer);
            memset(buffer,0,rbytes);
          }
        }
      }
    }
  }
  // connection finished
  memset(buffer,0,sizeof(buffer));
  printf("[+] connection closed\n");
  return 0;
}
