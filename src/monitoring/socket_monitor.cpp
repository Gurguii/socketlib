#include "socket_monitor.hpp"

uint32_t operator|(SOCKET_EVENTS lev,SOCKET_EVENTS rev){
  return (uint32_t)((uint32_t)lev | (uint32_t)rev);
}
bool operator&(SOCKET_EVENTS lev, SOCKET_EVENTS rev){
  return (uint32_t)lev & (uint32_t)rev;
}

namespace gsocket::monitoring
{
  SocketMonitor::SocketMonitor():_epoll(epoll_create1(0)){
  }
  
  void SocketMonitor::add(int fd, EVENTS evs){
    //epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
    epoll_event nfd{
    .events = static_cast<uint32_t>(evs),
    };
    nfd.data.fd = fd;
    epoll_ctl(_epoll,EPOLL_CTL_ADD,fd,&nfd);
    ++managed_descriptors;
  }
  void SocketMonitor::add(__base_socket *socket, EVENTS events){
    epoll_event nfd{
      .events = (uint32_t)events,
    };
    nfd.data.fd = socket->_fd; 
    epoll_ctl(_epoll,EPOLL_CTL_ADD,socket->_fd,&nfd);
    ++managed_descriptors;
  } 
  void SocketMonitor::remove(int fd){
    epoll_ctl(_epoll,EPOLL_CTL_DEL,fd,nullptr);
  }
  void SocketMonitor::remove(__base_socket *socket){
    epoll_ctl(_epoll,EPOLL_CTL_DEL,socket->_fd,nullptr);
  }
  EVENTS SocketMonitor::get_events(gsocket::__base_socket *socket){
    uint32_t evs = 0;  
    for(auto iter = available_events.begin(); iter != available_events.end(); iter++){
      if(iter->data.fd == socket->_fd){
        evs |= iter->events;
      }
    } 
    return (EVENTS)evs;
  } 
  int SocketMonitor::wait_events(int max_events,int timeout){
    if(available_events.size() < max_events){
      available_events.reserve(max_events);
    }
    memset(&available_events[0],0,available_events.size()*sizeof(epoll_event));
    // epoll_even events[desired_events]
    //timespec t{
    //.tv_sec = -1
    //};
    //t.tv_nsec = 0;
    //epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
    //epoll_pwait2(int epfd, struct epoll_event *events, int maxevents, const struct timespec *timeout, const __sigset_t *ss)
    int rcode = epoll_wait(_epoll,&available_events[0],max_events,(timeout > 0 ? timeout * 1000 : -1)); 
    // TODO - make epoll_pwait2() work
    //int a = epoll_pwait2(_epoll,&events[0],managed_descriptors,&t,NULL); 
    return rcode;
  }
  void SocketMonitor::clear_events(){
    memset(&available_events[0],0,sizeof(epoll_event) * available_events.size());
  }
}
