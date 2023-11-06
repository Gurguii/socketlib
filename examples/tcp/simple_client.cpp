#include <gsocket/gsocket.hpp>
using namespace gsocket;
using namespace gsocket::monitoring;
int main()
{
  tcp4socket sock;
  if(sock.connect("127.0.0.1", 9001)){
    std::cerr << sockError << "\n";
    return -1;
  }
  File passwd{"/etc/passwd"};
  uint64_t sbytes = sock.send(passwd);
  if(sbytes <= 0){
    fprintf(stderr,"%s\n",sockError);
    return -1;
  }
  printf("sent %lu bytes\n", sbytes);
  return 0;
}
