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
  
### Simple tcp(Ipv4) client server example ###
```cpp
//server.cpp
#include <gsocket/tcp_classes/tcpServer>
int main(){
    // store data
    std::string buffer; 
    // tcpServer constructor binds it to addr port
    tcpServer sv("127.0.0.1",8080); 
    // function blocks till connection arrives
    auto client = sv.acceptConnection(); 
    // function blocks until data received or error/connection closed 
    client.awaitData(buffer);
    std::cout << "received: " << buffer << "\n";
} 
```
```cpp
//client.cpp
#include <gsocket/tcp_classes/tcpClient>
int main(){
    // tcpClient constructor connects to addr port
    tcpClient cl("127.0.0.1",8080);
    // send data
    cl.send("hello from client\n");
    // close the socket's file descriptor
    cl.close();
}
```  
Note that this is a super **simple** and straightforward example for both server and client, if you want to see a **more robust** approach (**checking** if binding/connection **succeed**) check **examples**.  
## Uninstalling  
Inside the **scripts folder** there are shell scripts for installing, uninstalling and reinstalling.
