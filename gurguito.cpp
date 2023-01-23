#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <ifaddrs.h>
#include <netdb.h>
#include <net/if.h>
#include <errno.h>
#include <iostream> // TEMP
#include <memory>

class addressInfo{
    private:
    std::unique_ptr<addrinfo> head;
    public:
    addressInfo(addrinfo *addrs)
    :head(std::make_unique<addrinfo>(*addrs))
    {
        // constructor
    }
    ~addressInfo(){
        printf("Destroying addressInfo object & freeing internal addrinfo\n");
        freeaddrinfo((addrinfo *)head.get());
    }
};

int main(){
	return 0;
}
