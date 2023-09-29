#pragma once

// CPP HEADERS
#include <string_view>
#include <string>
#include <memory>

// C HEADERS
#include <unistd.h>

// CUSTOM HEADERS
#include "../../utils/fd.hpp"

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
    /**
     * @brief getters for reader fd
     * @return the file descriptor
     */
    int const GetReader();
    /**
     * @brief getter for writer fd
     * @return the file descriptor
     */
    int const GetWriter();
    /**
     * Closes the reader.
     *
     * This function closes the reader associated with the pipe.
     *
     * @return 0 on success, -1 on error.
     */
    int closeReader();
    /**
     * Closes the writer.
     *
     * This function closes the writer associated with the pipe.
     *
     * @return 0 on success, -1 on error.
     */
    int closeWriter();
    /**
     * Closes the pipe.
     *
     * This function closes the pipe.
     *
     * @return 0 on success, -1 on error.
     */
    int close();
    /**
    * @brief write data to the pipe
    * @return amount of bytes written
    */
    int write(std::string_view data);
    /**
    * @brief read N bytes of data to buffer, being N the buffer size
    * @return amount of bytes read
    */
    int read(std::string &buffer);
    /**
    * @return amount of bytes available to read in pipe
    */
    int available2read();
  };
}
