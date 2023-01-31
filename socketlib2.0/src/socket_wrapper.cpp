#include "socket_wrapper.hh"
constexpr int __TIMEOUT_MULTIPLIER = 1000;
namespace gsocket{
    /* __sw C'tors */
    __sw::__sw(Domain d, Type t, Behaviour b = BLOCK):domain(static_cast<int>(d)), type(b == BLOCK ? static_cast<int>(t) : static_cast<int>(t) | SOCK_NONBLOCK),fd(socket(domain,type,0)){}
    __sw::__sw(uint8_t d, uint8_t t, uint8_t p):domain(d),type(t),fd(socket(d,t,p)){}
    __sw::__sw(uint8_t fd):fd(fd){}
    /* CLOSE */
    int __sw::close(){
        return ::close(fd);
    }
    /* CONNECT FUNCTIONS */
    int __sw::connect(std::string_view h, uint16_t p){
        sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        addr.sin_family = domain;
        addr.sin_port = htons(p);
        if(inet_pton(domain, h.data(), &addr.sin_addr) <= 0){
            return 1;
        }
        return ::connect(fd, reinterpret_cast<sockaddr*>(&addr), addrlen); 
    }
    int __sw::connect(addrinfo *const addr){
        return ::connect(fd, addr->ai_addr, addr->ai_addrlen);
    }
    int __sw::connect(addressInfo *xdd){
        return 1;
    }
    /* SEND FUNCTIONS */
    int __sw::send(std::string_view d){
        return ::send(fd, d.data(), d.size(), 0);
    }
    int __sw::send(std::string_view d, int b){
        return ::send(fd, d.data(), b, 0);
    };
    /* RECV FUNCTIONS */
    std::optional<std::string> __sw::recv(){
        std::string buffer(__IO_BUFFSIZE,'\x00');
        int n = ::recv(fd, buffer.data(), __IO_BUFFSIZE, 0);
        if(n == -1 || n == 0){
            return{};
        }
        if(n == __IO_BUFFSIZE){
            int avbytes;
            ioctl(fd, FIONREAD, &avbytes);
            buffer.resize(__IO_BUFFSIZE+avbytes);
            ::recv(fd, &buffer[__IO_BUFFSIZE], avbytes, 0);
        }
        return buffer;
    }
    std::optional<std::string> __sw::recv(int bytes){
        std::string buffer(bytes,'\x00');
        if(::recv(fd, buffer.data(), bytes, 0) == -1){
            return {};
        };
        return buffer;
    }
    /* AWAIT FUNCTIONS */
    template <typename T> int __sw::awaitData(T &buff, int tout){
        auto fdpol = pollfd{
            .fd = fd,
            .events = POLLIN
        };
        int r = poll(&fdpol,1,(tout > 0 ? tout * __TIMEOUT_MULTIPLIER : -1));
        if(r > 0 && (fdpol.revents & POLLIN)){
            int b;
            ioctl(fd, FIONREAD, &b);
            if(buff.size() < b){
                buff.resize(buff.size()+b);
            }
            return ::recv(fd, buff.data(), buff.size(), 0);
        }
        return -2;
    }
    int __sw::awaitDataFrom(msgFrom &__data, int timeout = -1){
        auto _s_poll = pollfd{
                .fd = fd,
                .events = POLLIN
            };
            int event, avBytes, rStatus;
            event = poll(&_s_poll, 1, (timeout == -1 ? -1 : (timeout * __TIMEOUT_MULTIPLIER)));
            if(event > 0 && (_s_poll.revents & POLLIN)){
                // data available
                ioctl(fd, FIONREAD, &avBytes);
                if(__data.msg.size() < avBytes){
                    __data.msg.resize(avBytes);
                }
                sockaddr addr;
                socklen_t &&addrlen = sizeof(addr);
                int &&n = ::recvfrom(fd, __data.msg.data(), avBytes, 0, &addr, &addrlen);
                if(n != -1){
                    __data.host = inet_ntoa(reinterpret_cast<sockaddr_in*>(&addr)->sin_addr);
                    __data.port = htons(reinterpret_cast<sockaddr_in*>(&addr)->sin_port);
                }
                return n;
                //return ::recv(sock, __sockHostData->msg.data(), avBytes, 0);
            }
            // timeout
            return -2;
    }
    /* BIND FUNCTIONS */
    int __sw::bind(std::string_view h, uint16_t p){
        sockaddr_in addr{
            .sin_family = domain,
            .sin_port = htons(p)
        };
        if(inet_pton(domain, h.data(), &addr.sin_addr) <= 0){
            return 1;
        }
        if(::bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr))){
            return 1;
        }
        return 0;
    }
    int __sw::bind(uint16_t p){
        sockaddr_in addr{
            .sin_family = domain,
            .sin_port = htons(p),
            .sin_addr = INADDR_ANY
        };
        if(::bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr))){
            return 1;
        };
        return 0;
    }
    /* LISTEN FUNCTIONS */
    int __sw::listen(int __mConns = 3){
        return ::listen(fd, __mConns);
    }
    /* ACCEPT FUNCTIONS */
    int __sw::accept(){
        sockaddr addr;
        socklen_t addrlen = sizeof(addr);
        return ::accept(fd, &addr, &addrlen);
    }
    int __sw::accept(Address& a){
        sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        uint8_t n = ::accept(fd, reinterpret_cast<sockaddr*>(&addr), &addrlen);
        a.host = inet_ntoa(addr.sin_addr);
        a.port = htons(addr.sin_port);
        return n;
    }
    /* RETRIEVE SOCKET DATA */
    Address __sw::getpeername(){
        sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        ::getpeername(fd, reinterpret_cast<sockaddr*>(&addr), &addrlen);
        return {inet_ntoa(addr.sin_addr), htons(addr.sin_port)};
    }
    Address __sw::getsockname(){
        sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        ::getsockname(fd, reinterpret_cast<sockaddr*>(&addr), &addrlen);
        return {inet_ntoa(addr.sin_addr), htons(addr.sin_port)};
    }
}