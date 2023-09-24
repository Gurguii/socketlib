#ifndef GSOCKET_HH
#define GSOCKET_HH
#include "core/socket_wrapper.hh"
/* dup3 FLAGS:
  O_RDONLY: Opens the file for reading only.
  O_WRONLY: Opens the file for writing only.
  O_RDWR: Opens the file for both reading and writing.
  O_CREAT: Creates the file if it doesn't exist.
  O_EXCL: Used with O_CREAT, ensures that open() fails if the file already exists.
  O_TRUNC: Truncates the file to zero length if it exists and is opened for writing.
  O_APPEND: Appends data to the end of the file instead of overwriting it.
  O_NONBLOCK: Sets the file descriptor to non-blocking mode. I/O operations return immediately if they can't be completed.
  O_SYNC: Writes are synchronized to the underlying storage device.
  O_DIRECTORY: Ensures that the opened file is a directory.
  O_NOFOLLOW: Prevents open() from following symbolic links.
  O_CLOEXEC: Closes the file descriptor automatically on exec calls.
  O_DIRECT: Bypasses the file system cache for I/O operations.
  O_NOATIME: Prevents updating the file access time.
  O_TMPFILE: Creates a temporary file that is automatically deleted when closed.
*/
class Socket : public gsocket::__sw {
public:
  Socket(Domain d, Type t, Behaviour b);
  Socket(uint8_t dom, uint8_t typ, uint8_t b);
  Socket(uint8_t fd);
  /* returns duplicated `Socket` instance's fd */ 
  Socket dup();
  Socket dup2(int newfd);
  Socket dup3(int newfd, int flags);
};
#endif
