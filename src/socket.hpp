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
#pragma once
#include "core/socket_wrapper.hpp"

/**
 * @defgroup socket Socket class
 * This group contains the Socket class, which represents a network socket.
 * @{
 */
/**
 * @class Socket
 * Class representing a network socket.
 *
 * The Socket class provides a simple and easy-to-use interface for creating, managing, and using network sockets. It supports both TCP and UDP sockets, as well as a variety of other features, such as non-blocking sockets, bound sockets, and connected sockets.
 *
 * To create a new Socket object, simply call the Socket() constructor with the desired domain, type, and behaviour. For example, to create a new TCP socket, you would use the following code:
 *
 * ```c++
 * Socket socket(Domain::ipv4, Type::tcp, Behaviour::block);
 * 
 *
 * Once you have created a Socket object, you can use it to perform a variety of operations, such as binding the socket to a specific address and port, connecting the socket to a remote server, and sending and receiving data over the socket.
 *
 * For more information on how to use the Socket class, please see the documentation for the individual methods.
 */
struct Socket : public gsocket::__socket_operations {
  /**
   * Constructs a new Socket object.
   *
   * @param d The domain of the socket.
   * @param t The type of the socket.
   * @param b The behaviour of the socket.
   */
  Socket(Domain d, Type t, Behaviour b);

  /**
   * Constructs a new Socket object.
   *
   * @param dom The domain of the socket (as a uint8_t).
   * @param typ The type of the socket (as a uint8_t).
   * @param b The behaviour of the socket (as a uint8_t).
   */
  Socket(uint8_t dom, uint8_t typ, uint8_t b);

  /**
   * Constructs a new Socket object from an existing file descriptor.
   *
   * @param fd The file descriptor of the socket.
   */
  Socket(uint8_t fd);

  /**
   * Returns a duplicate of the Socket object's file descriptor.
   *
   * @return A duplicate of the Socket object's file descriptor.
   */
  Socket dup();

  /**
   * Duplicates the Socket object's file descriptor to a new file descriptor.
   *
   * @param newfd The new file descriptor.
   * @return The Socket object.
   */
  Socket dup2(int newfd);

  /**
   * Duplicates the Socket object's file descriptor to a new file descriptor, with the given flags.
   *
   * @param newfd The new file descriptor.
   * @param flags The flags to use when duplicating the file descriptor.
   * @return The Socket object.
   */
  Socket dup3(int newfd, int flags);
};

/** @} */ // end of socket group
