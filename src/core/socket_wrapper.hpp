#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <ifaddrs.h>
#include <iostream>
#include <net/if.h>
#include <netdb.h>
#include <optional>
#include <string>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
// TESTING
#include <future>
#include <thread>
// TESTING
#include "../enums.hpp"
#include "../structs.hpp"
#include "../utils/fd.hpp"
#include "../utils/socket.hpp"
#include "../utils/net.hpp"

#define sockError strerror(errno)
#define __IO_BUFFSIZE 4096

namespace gsocket {
  /**
   * @brief The base socket wrapper class.
   */
  class __socket_operations {
  protected:
    /**
     * @brief Constructor with domain, type, and behavior parameters.
     * @param d Domain of the socket.
     * @param t Type of the socket.
     * @param b Behavior of the socket.
     */
    __socket_operations(Domain d, Type t, Behaviour b);
    
    /**
     * @brief Constructor with domain, type, and protocol parameters.
     * @param domain Domain of the socket.
     * @param type Type of the socket.
     * @param protocol Protocol of the socket.
     */
    __socket_operations(uint8_t domain, uint8_t type, uint8_t protocol);
    
    /**
     * @brief Constructor with file descriptor parameter.
     * @param fd File descriptor of the socket.
     */
    __socket_operations(uint8_t fd);
    
    /**
     * @brief Destructor for the socket wrapper class.
     */
    ~__socket_operations();

  public:
    uint8_t domain, type, fd;
    
    /**
     * @brief Get the file descriptor of the socket.
     * @return The file descriptor.
     */
    int const getFD();

    /**
     * @brief Close the socket.
     * @return 0 on success, -1 on failure.
     */
    int close();
    /**
     * @defgroup connect_overloads
     * @brief overloads of the POSIX connect() function
     *
     * @addtogroup connect_overloads
     * @{ */
  //TESTING
  std::future<std::string> connectAsync(const char *host, int port)
  {
    auto a = std::async(std::launch::async,gsocket::getIpByIface,"vbr0",Domain::ipv4);
    return a;
  }
    /**
     * @brief Connect to a remote host.
     * @param host The host to connect to.
     * @param port The port to connect to.
     * @return 0 on success, -1 on failure.
     */
    int connect(const char*& host, int& port);

    /**
     * @brief Move version of the connect function.
     * @param host The host to connect to.
     * @param port The port to connect to.
     * @return 0 on success, -1 on failure.
     */
    int connect(const char*&& host, int&& port);

    /**
     * @brief Connect to a remote host using a given address.
     * @param addr The socket address to connect to.
     * @return 0 on success, -1 on failure.
     */
    int connect(addrinfo* const addr);

    /**
     * @brief Connect to a remote host using a given address info.
     * @param addr The address info to connect to.
     * @return 0 on success, -1 on failure.
     */
    int connect(addressInfo* addr);

    /**
     * @brief Connect to a remote host using a given address and length.
     * @param addr The address to connect to.
     * @param addrlen The length of the address.
     * @return 0 on success, -1 on failure.
     */
    int connect(sockaddr* addr, socklen_t& addrlen);

    /**
     * @brief Connect to a remote host using a given host and port.
     * @param host The host to connect to.
     * @param port The port to connect to.
     * @return 0 on success, -1 on failure.
     */
    int connect(std::string_view& host, uint16_t& port);

    /**
     * @brief Perfect forwarding for the connect function.
     * @tparam Args The argument types.
     * @param args The arguments.
     * @return 0 on success, -1 on failure.
     */
    //template <typename... Args>
    //int connect(Args&&... args);
    /**@}*/
    
    /**
     * @defgroup connect_overloads
     * @brief overloads of the POSIX connect() function
     *
     * @addtogroup connect_overloads
     * @{ */
    /**
     * @brief Send data through the socket.
     * @param data The data to send.
     * @return The number of bytes sent, or -1 on failure.
     */
    int send(std::string_view data);

    /**
     * @brief Send data through the socket and get the number of bytes sent.
     * @param data The data to send.
     * @param bytes The number of bytes sent.
     * @return The number of bytes sent, or -1 on failure.
     */
    int send(std::string_view data, int& bytes);

    /**
     * @brief Move version of the send function.
     * @param data The data to send.
     * @param bytes The number of bytes sent.
     * @return The number of bytes sent, or -1 on failure.
     */
    int send(std::string_view data, int&& bytes);

    /**
     * @brief Send data through the socket as a C-style string.
     * @param data The data to send.
     * @return The number of bytes sent, or -1 on failure.
     */
    int send(const char*& data);

    /**
     * @brief Move version of the send function for C-style strings.
     * @param data The data to send.
     * @return The number of bytes sent, or -1 on failure.
     */
    int send(const char*&& data);

    /**
     * @brief Send data through the socket as a string.
     * @param data The data to send.
     * @return The number of bytes sent, or -1 on failure.
     */
    int send(std::string& data);

    /**
     * @brief Perfect forwarding for the send function.
     * @tparam Args The argument types.
     * @param args The arguments.
     * @return The number of bytes sent, or -1 on failure.
     */
    template <typename... Args>
    int send(Args&&... args);
    /**@}*/
    
    /**
     * @defgroup connect_overloads
     * @brief overloads of the POSIX connect() function
     *
     * @addtogroup connect_overloads
     * @{ */
    /**
     * @brief Send data to a specific host and port.
     * @param host The host to send data to.
     * @param port The port to send data to.
     * @param msg The message to send.
     * @return The number of bytes sent, or -1 on failure.
     */
    int sendto(std::string_view host, uint16_t port, std::string_view msg);

    /**
     * @brief Perfect forwarding for the sendto function.
     * @tparam Args The argument types.
     * @param args The arguments.
     * @return The number of bytes sent, or -1 on failure.
     */
    template <typename... Args>
    int sendto(Args&&... args);
    /**@}*/
    
    /**
     * @defgroup connect_overloads
     * @brief overloads of the POSIX connect() function
     *
     * @addtogroup connect_overloads
     * @{ */
    /**
     * @brief Receive all available data from the socket.
     * @return The received data as a string.
     */
    std::string recv();

    /**
     * @brief Receive a specified number of bytes from the socket.
     * @param bytes The number of bytes to receive.
     * @return The received data as a string.
     */
    std::string recv(int bytes);

    /**
     * @brief Receive a specified number of bytes into a buffer.
     * @param buffer The buffer to receive the data.
     * @param bytes_to_read The number of bytes to read.
     * @return The number of bytes received, or -1 on failure.
     */
    int recv(char* buffer, size_t bytes_to_read);
    /**@}*/

    /**
     * @brief Receive data and populate the given msgFrom struct.
     * @param data The msgFrom struct to populate.
     * @return The number of bytes received, or -1 on failure.
     */
    int recvfrom(msgFrom& data);

    /**
     * @brief Wait for data in the socket for a specified timeout.
     * @param buffer The buffer to store the received data.
     * @param timeout The timeout duration in seconds.
     * @return -1 on errors, -2 on timeout, or the amount of bytes read.
     */
    int awaitData(std::string& buffer, int timeout = -1);

    /**
     * @brief Wait for data from a specific host for a specified timeout.
     * @param __sockHostData The msgFrom struct to populate.
     * @param timeout The timeout duration in seconds.
     * @return -1 on errors, -2 on timeout, or the amount of bytes read.
     */
    int awaitDataFrom(msgFrom& __sockHostData, int timeout);

    /**
     * @brief Bind the socket to a specific address and port.
     * @param addr The address to bind to.
     * @param port The port to bind to.
     * @return 0 on success, -1 on failure.
     */
    int bind(std::string_view addr, uint16_t port);

    /**
     * @brief Bind the socket to a specific port.
     * @param port The port to bind to.
     * @return 0 on success, -1 on failure.
     */
    int bind(uint16_t port);

    /**
     * @brief Perfect forwarding for the bind function.
     * @param Args The argument types.
     * @param args The arguments.
     * @return 0 on success, -1 on failure.
     */
    template <typename... Args>
    int bind(Args&&... args);

    /**
     * @brief Listen for incoming connections.
     * @param maxConns The maximum number of connections to allow.
     * @return 0 on success, -1 on failure.
     */
    int listen(int maxConns = 3);

    /**
     * @brief Accept an incoming connection.
     * @return The file descriptor of the accepted connection, or -1 on failure.
     */
    int accept();
    /**
    * @brief Accept an incoming conection.
    * @return The file descriptor of the accepted connection, or -1 on failure.
    */
    template <typename AddrStruct> int accept(AddrStruct &a);
    /**
    * @brief Get socket host and port
    * @return `struct Addr6`
    */
    Addr6 getsockname();
    /**
    * @brief Get socket's peer host and port
    * @return `struct Addr6`
    */
    Addr6 getpeername();
  };
}
