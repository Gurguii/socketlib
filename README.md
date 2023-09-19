# [POSIX] TCP/UDP & IPV4/IPV6 socket library
## Description  
#### C++ library that wraps some of the POSIX socket methods, such as send, recv, bind, connect ... and adds some other custom ones such as awaitData, which allows adding a timeout.  
#### The idea is to have a library that allows doing stuff with sockets with less code, using OOP to *ideally* make decent reusable code.  
Note that I'm not an experienced C++ developer, even tho I want and try to make things efficient, I might be doing a complete catastrophe
## Setup  
**Clone de project**  
```bash
git clone https://github.com/Gurguii/socketlib
```  
**Get into the project directory**  
```bash
cd https://github.com/Gurguii/socketlib
```  
**Run the installation script**  
```bash
sudo bash ./scripts/install.sh
```  
Note that you can also manually install it, there is nothing special happening in the installation script   
 
## Using the library
**tcpclient.cpp**  
Description: Connect to target host  and port, then send 'Hello World!\' through the socket
```cpp 
//tcpclient.cpp
#include <gsocket/gsocket.hh>
int main(){
    // create tcp socket
    Socket client(inet, tcp, BLOCK);
    // connect to target   
    if(client.connect("127.0.0.1", 9001)){
        std::cerr << "couldn't connect to target\n";
        return 1;
    }
    // send data
    client.send("Hello World!\n");
    // close socket    
    client.close();
    return 0;
}
```  
**httpclient.cpp**  
Description: Send a blocking http GET request, wait response and print to console
```cpp
//httpclient.cpp  
#include <gsocket/gsocket.hh>

const char *host = "127.0.0.1";
int port = 5214;

const char *http_request_template =
    "GET /%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n";

int main(int argc, const char **args) {
  if (argc == 1) {
    std::cerr << "Usage: " << args[0] << " <endpoint>\n";
    return 1;
  }

  const char **endpoint = &args[1];

  // Create tcp blocking socket
  Socket httpclient(inet, tcp, BLOCK);

  // Try connecting to localhost
  if (httpclient.connect(host, port) == -1) {
    std::cerr << sockError << "\n";
    return 1;
  }

  // Create a string that will store our GET request
  std::string get_body(1024, '\x00');
  snprintf(&get_body[0], get_body.size(), http_request_template, *endpoint,
           HOST);

  // Send data through socket (if properly formatted, it will essencially be a
  // GET request)
  httpclient.send(get_body);

  // Receive response and store it in buffer
  std::string response = httpclient.recv();

  // Close socket
  httpclient.close();

  // Print response
  std::cout << "== RESPONSE==\n" << response << "\n";

  return 0;
}
```
## Uninstalling  
Inside the **scripts folder** there are bash scripts for installing, uninstalling and reinstalling libgsocket
