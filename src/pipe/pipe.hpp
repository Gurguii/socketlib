#pragma once

// CPP HEADERS
#include <string_view>
#include <string>
#include <memory>

// C HEADERS
#include <unistd.h>

// CUSTOM HEADERS
#include "../utils/fd.hpp"

namespace gsocket
{

/**
 * @brief class to work with pipes in an easier way
 */
class Pipe
{
  private:
    int reader;
    int writer;
  public:
    Pipe();
    ~Pipe();
    int const GetReader();
    int const GetWriter();
    int closeReader();
    int closeWriter();
    int close();
    int write(std::string_view data);
    int read(std::string &buffer);
    int read(char *buffer, uint64_t size);
    int available2read();
  };
}
