#include <gsocket/gsocket.hpp>
using namespace gsocket;
using namespace gsocket::monitoring;
int main()
{
  tcp4socket cl;
  if(cl.connect("127.0.0.1", 9001)){
    std::cerr << sockError << "\n";
    return -1;
  }
  if(fork() == 0){
    // child
    int code = 0;
    uint8_t buffer[1024 * 4 * 4]{0}; // 4MB buffer
    uint64_t rbytes = 0;
    memset(buffer,0,sizeof(buffer));
    SocketMonitor watcher;
    watcher.add(&cl,EVENTS::READABLE); 
    for(;;){
      code = watcher.wait_events();  
      if(code <= 0){
        break;
      } 
      EVENTS client_events = watcher.get_events(&cl); 
      if(!(client_events & EVENTS::READABLE)){           
        printf("not readable\n");
        break; 
      }
      uint64_t available_bytes = get_available_bytes2(&cl);
      printf("%lu bytes available\n",available_bytes);
      rbytes = cl.recv((char*)buffer,available_bytes);
    ;
  if(rbytes <= 0){
        // connection closed
        rbytes = 0;
        break;
      }
      printf("%s",buffer);     
      memset(buffer,0,available_bytes);       
      watcher.clear_events();
      rbytes = 0;
      available_bytes = 0;
    }
  }else{
    // parent
    char msg[1024];
    int read = 0;
    int sbytes = 0;
    memset(msg,0,sizeof(msg));
    for(;;){
      fgets(msg,sizeof(msg),stdin);
      sbytes = cl.send(msg,strlen(msg));
      if(sbytes <= 0){
        break;
      }
      printf("sent %i bytes\n",sbytes);
    }  
  }
}
