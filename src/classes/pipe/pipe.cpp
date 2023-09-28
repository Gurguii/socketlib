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
  int Pipe::write(std::string_view data)
  {
    return ::write(writer,data.data(),data.size());
  };
  int Pipe::read(std::string &buffer)
  {
    return ::read(reader, &buffer[0], buffer.size());
  }
  int Pipe::available2read()
  {
    std::unique_ptr<int> n = std::make_unique<int>();
    gsocket::availableBytes(reader,n.get()); 
    return std::move(*n);
  }
}