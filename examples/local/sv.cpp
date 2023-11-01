#include <gsocket/gsocket.hpp>
#include <signal.h>

void handler_ctrl_c(int signal){
  printf("[!] You pressed ctrl+c, you really want to exit? [y/n] ");
  char ans = getchar();
  if(ans == 'y' || ans == 'Y' ){
    printf("[+] Running cleanup ...\n");
    if(unlink("/tmp/gurguitest")){
      perror("removing /tmp/gurguitest ...\n");
      return;
    };
  }else{
    printf("[+] Staying ...\n");
  }
}

using namespace gsocket;
int main()
{
  // Add signal handler
  signal(SIGINT,handler_ctrl_c);

  localsocket sock("/tmp/gurguitest",ACTION::bind);
  char buff[2048];
  int rbytes;
  for(;;){
    printf("[+] accepting new connections ...\n");
    auto cl = sock.accept();
    rbytes = cl.recv(buff,sizeof(buff));
    if(rbytes > 0){
      printf("%i bytes : %s\n", rbytes,buff);
      memset(buff,0,rbytes);
    }
  }
  sock.close();
  return 0;
}
