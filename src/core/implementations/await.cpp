#include "../__base_socket.hpp"

namespace gsocket
{
 int __base_socket::await(std::string &buffer, int timeout, __sigset_t *_signals){
  int _watcher = epoll_create1(0);
  if(_watcher <= 0){
   return -1;
  } 
  epoll_event evs2track{EPOLLIN}; 
  if(epoll_ctl(_watcher, EPOLL_CTL_ADD, _fd, &evs2track) < 0){
   return -1;
  }
  epoll_event ev;

  // wait for the event
  timespec _timeout{.tv_sec = timeout};

  // TODO - implement __sigset_t ( WRAP IT )
  // __sigset_t ss;
  // sigemptyset(&ss);
  // sigaddset(&ss,SIGINT);
  
  // TODO - fix return values
  int err;
  if((err = epoll_pwait2(_watcher, &ev, 1, &_timeout, _signals)) <= 0){
    // 0 - timeout 
    // -1 - error
    return err; 
  };
  if(ev.events == EPOLLIN){
   // data available
   uint32_t _avbytes;
   ioctl(_fd,FIONREAD,&_avbytes);
   if(_avbytes > 0){
    if(buffer.size() < _avbytes){
     buffer.resize(_avbytes);
    }
    return ::read(_fd,&buffer[0],buffer.size());
   }
   return -1; 
  }
  return 0;
 }

 int __base_socket::await(void *buffer, uint64_t nbytes, int timeout, __sigset_t *_signals){
  int _watcher = epoll_create1(0);
  if(_watcher < 0){ return -1;}
  epoll_event event{ .events = EPOLLIN};
  if(epoll_ctl(_watcher, EPOLL_CTL_ADD, _fd, &event) < 0){
   return -1;
  };
  timespec time{.tv_sec = timeout};
  epoll_event available;
  if(epoll_pwait2(_watcher, &available, 1, &time, _signals) <= 0){
   printf("pwait2()\n");
   return -1;
  }
  if(available.events == EPOLLIN){
   // got data available
   return ::read(_fd,buffer, nbytes);
  }
  return -1;
 };
}
