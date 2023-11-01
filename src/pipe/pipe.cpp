#include "pipe.hpp"

namespace gsocket
{
  Pipe::Pipe(){
    int fds[2];
    pipe(fds);
    reader = std::move(fds[0]);
    writer = std::move(fds[1]);
  }
  Pipe::~Pipe(){
    ::close(reader);
    ::close(writer);
  }
  int const Pipe::GetReader(){
    return reader;
  }
  int const Pipe::GetWriter(){
    return writer;
  }
  int Pipe::closeWriter(){
    return ::close(writer);
  }
  int Pipe::closeReader(){
    return ::close(reader);
  }
  int Pipe::close(){
    return (::close(writer) || ::close(reader) ? -1 : 0);
  }
  int Pipe::write(std::string_view data)
  {
    return ::write(writer,data.data(),data.size());
  };
  int Pipe::read(std::string &buffer)
  {
    return ::read(reader, &buffer[0], buffer.size());
  }
  int Pipe::read(char *buffer, uint64_t size){
    return ::read(reader, buffer, size);
  };
  int Pipe::available2read()
  {
    std::unique_ptr<int> n = std::make_unique<int>();
    get_available_bytes(reader,n.get()); 
    return std::move(*n);
  }
}
