#pragma once
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <uuid/uuid.h>
#include <unistd.h>
#include <fcntl.h>



namespace gsocket
{
 class NamedPipe
  {
  private:
    std::string path;
    int fd;
    static std::string generate_uuid();
  public:
    NamedPipe();
    NamedPipe(std::string_view filename);
    ~NamedPipe();

    int write(std::string_view data);
    int read(std::string &buffer);
    std::string_view getPath();
  };
}
