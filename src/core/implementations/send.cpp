#include "../__base_socket.hpp"
#include "../../../testing/bufferclass.cpp"

namespace gsocket
{
 
 int __base_socket::send(std::string_view data){
  return ::send(_fd,data.data(),data.size(),0);
 }

 int __base_socket::send(char *buffer, size_t size){
  return ::send(_fd,buffer,size,0); 
 }

 int __base_socket::send(FILE *file){
  fseek(file,0,SEEK_END);
  uint64_t size = ftell(file);
  fseek(file,0,SEEK_SET);
  testing::Buffer buffer(size);
  if(fread(buffer.beg(),size,1,file) <= 0){
   return -1;
  };
  int sbytes = ::send(_fd,buffer.beg(),size,0); 
  return sbytes;
 }

 int __base_socket::send(File &file){
  FILE *_file = fopen(file.path,"rb");
  if(_file == nullptr){
   return -1;
  }
  int _return = send(_file); 
  fclose(_file);
  return _return;
 }
}
