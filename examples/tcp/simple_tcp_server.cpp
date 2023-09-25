/* @section tcp_examples 
 * @title simple tcp server example
 * @brief simple socket server that receives a connection and waits for
 * TIMEOUT seconds to receive a message, it then prints the received data and closes the socket
 * @note if TIMEOUT seconds pass, the client socket will be closed and a new connection will be accepted */
#include <gsocket/gsocket.hh>

constexpr int TIMEOUT = 10;
constexpr int MAX_CONNECTIONS = 10;

int main(int argc, const char **args)
{
  Socket sv(ipv4,tcp,block);
  if(sv.bind((uint16_t)9001)){
    std::cerr << "couldn't bind - " << sockError << "\n";
    return -1;
  }
  const auto [lhost,lport] = sv.getsockname();
  std::cout << "LISTENING ON " << lhost << " " << lport << "\n";

  if(sv.listen(MAX_CONNECTIONS)){
    std::cerr << "couldn't start listening - " << sockError << "\n";
    return -1;
  }

  // buffer to store incoming data
  std::string buffer(4096,'\x00');
  int ans;
  
  // loop permanently
  for(;;){
    // accept incoming connection
    Socket client = sv.accept();
    std::cout << "INCOMING CLIENT\n";
    // await data
    ans = client.awaitData(buffer,TIMEOUT);
    // check returned code and act accordingly
    switch(ans){
      case -1:
      std::cerr << "error - " << sockError << "\n";
      break;
      case -2:
      std::cerr << "Client timeout\n";
      break;
      default:
      std::cout << "Received: " << buffer << "\n";
      break;
    }
    // close client connection before accepting a new one
    client.close();
    // empty buffer
    buffer.clear();
  }
  return 0;  
}
