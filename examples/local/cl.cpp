#include <gsocket/gsocket.hpp>
using namespace gsocket;
int main()
{
  localsocket client("/tmp/gurguitest",ACTION::connect);
  char buff[1024];
  int wbytes;
  for(;;){
    fgets(buff,1024,stdin);
    wbytes = client.send(buff,sizeof(buff));
    if(wbytes < 0){
      break;
    }else{
      printf("sent %lu bytes\n",strlen(buff));
      memset(buff,0,strlen(buff));
    }
  }
  client.close();
  return 0;
}
