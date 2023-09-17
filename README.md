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
Note that this is a super **simple** and straightforward example for both server and client, if you want to see a **more robust** approach (**checking** if binding/connection **succeed**) check **examples**.  
## Uninstalling  
Inside the **scripts folder** there are shell scripts for installing, uninstalling and reinstalling.
