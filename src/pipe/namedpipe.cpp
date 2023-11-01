#include "namedpipe.hpp"
#include <iostream>
#include <fcntl.h>
#include <filesystem>
namespace gsocket
{

std::string NamedPipe::generate_uuid(){
  uuid_t uuid;
  uuid_generate(uuid);
  char str_uuid[37];
  uuid_unparse(uuid,str_uuid);
  return std::move(str_uuid);
}
  NamedPipe::NamedPipe(){
    std::string filename = generate_uuid();    
    path = mkfifo(filename.c_str(),S_IRWXU) ? nullptr : filename;
  }

  NamedPipe::NamedPipe(std::string_view filepath){
    if(!mkfifo(filepath.data(),S_IRWXU)){
      path = filepath;  
    }
    if((fd = open(filepath.data(),O_RDWR,0)) <= 0){
    }
  };

  NamedPipe::~NamedPipe(){
    ::close(fd);
    if(std::filesystem::exists(path)){
      std::filesystem::remove(path);
    }
  }

  int NamedPipe::write(std::string_view data){
    return ::write(fd,data.data(),data.size());
  }

  int NamedPipe::read(std::string &buffer){
    return ::read(fd,&buffer[0],buffer.size());
  }

  std::string_view NamedPipe::getPath(){
    return path;
  }
}
