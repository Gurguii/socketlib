#include "localsocket.hpp"

namespace gsocket
{
localsocket::~localsocket(){ 
  if(is_server){
    unlink(sun_path);
  }
}
localsocket::localsocket(int fd):__base_socket(fd),is_server(0){domain = AF_UNIX;};
localsocket::localsocket(const char *path, ACTION action, TYPE type, BEHAVIOUR behaviour)
:__base_socket(socket(AF_UNIX,(behaviour == BEHAVIOUR::block 
                      ? (int)type
                      : (int)type | SOCK_NONBLOCK),0)),is_server(action == ACTION::bind ? 1 : 0){
  strcpy(sun_path,path);
  sun_family = AF_UNIX;
  // TODO - change this dogshit error print
  if(action == ACTION::bind){
    if(bind()){
      std::cerr << "bind() - " << sockError << "\n";
    };
  }else{
    if(connect()){
      std::cerr << "connect() - " << sockError << "\n";
    };
  }
}

int localsocket::connect(){
  return ::connect(_fd, (sockaddr*)this, sizeof(struct sockaddr_un));
}

int localsocket::bind(){
  if(::bind(_fd,(sockaddr*)this,sizeof(struct sockaddr_un))){
    return -1;
  }
  if(::listen(_fd,3)){
    return -1;
  }; 
  return 0;
}

localsocket localsocket::accept(){
  return localsocket(::accept(_fd,nullptr,nullptr));
}
}
