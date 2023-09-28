#pragma once 
#include "../src/socket.hh"
/**
 * @brief Connects the gsocketClass object to the specified address and port.
 * 
 * @tparam gsocketClass The type of gsocketClass object.
 * @param ptr Pointer to the gsocketClass object.
 * @param addr The address to connect to.
 * @param port The port number to connect to.
 * @return int Returns an integer representing the success or failure of the operation.
 */
template <class gsocketClass>
int connect(gsocketClass *ptr, std::string_view addr, uint16_t port);

/**
 * @brief Sends data using the gsocketClass object.
 * 
 * @tparam gsocketClass The type of gsocketClass object.
 * @param ptr Pointer to the gsocketClass object.
 * @param data The data to send.
 * @return int Returns an integer representing the success or failure of the operation.
 */
template <class gsocketClass>
int send(gsocketClass *ptr, std::string_view data);

/**
 * @brief Receives data using the gsocketClass object.
 * 
 * @tparam gsocketClass The type of gsocketClass object.
 * @tparam A The type of the buffer.
 * @param ptr Pointer to the gsocketClass object.
 * @param buff Pointer to the buffer to store the received data.
 * @return int Returns an integer representing the success or failure of the operation.
 * 
 * Note: This template function does not empty the buffer, which means previous data might be displayed if you don't empty it somehow (e.g., clear(), assign("")). Example: first read -> "gurgui" || second read -> "aaa" || buff -> aaagui
 */
template <class gsocketClass, class A>
int recv(gsocketClass *ptr, A *buff);

/**
 * @brief Waits for data to arrive using the gsocketClass object.
 * 
 * @tparam gsocketClass The type of gsocketClass object.
 * @tparam A The type of the buffer.
 * @param ptr Pointer to the gsocketClass object.
 * @param buff Pointer to the buffer to store the received data.
 * @param timeout The timeout value in milliseconds (default: -1 for indefinite timeout).
 * @return int Returns an integer representing the success or failure of the operation.
 */
template <class gsocketClass, class A>
int awaitData(gsocketClass *ptr, A *buff, int timeout = -1);

/**
 * @brief Binds the gsocketClass object to the specified address and port.
 * 
 * @tparam gsocketClass The type of gsocketClass object.
 * @param ptr Pointer to the gsocketClass object.
 * @param h The host address to bind to.
 * @param port The port number to bind to.
 * @return int Returns an integer representing the success or failure of the operation.
 */
template <class gsocketClass>
int bind(gsocketClass *ptr, std::string_view h, uint16_t port);

/**
 * @brief Listens for incoming connections using the gsocketClass object.
 * 
 * @tparam gsocketClass The type of gsocketClass object.
 * @param ptr Pointer to the gsocketClass object.
 * @param __maxconns The maximum number of connections to allow (default: 3).
 * @return int Returns an integer representing the success or failure of the operation.
 */
template <class gsocketClass>
int listen(gsocketClass *ptr, int __maxconns = 3);

/**
 * @brief Accepts an incoming connection using the gsocketClass object.
 * 
 * @tparam gsocketClass The type of gsocketClass object.
 * @param ptr Pointer to the gsocketClass object.
 * @param buff Pointer to the Addr6 object to store the address of the connected client (optional).
 * @return Socket Returns a Socket object representing the accepted connection.
 */
template <class gsocketClass>
Socket acceptConnection(gsocketClass *ptr, Addr6 *buff = nullptr);

/**
 * @brief Retrieves the local address associated with the gsocketClass object.
 * 
 * @tparam gsocketClass The type of gsocketClass object.
 * @param ptr Pointer to the gsocketClass object.
 * @param buff Pointer to the Addr6 object to store the local address (optional).
 * @return std::optional<Addr6> Returns an optional Addr6 object representing the local address, or std::nullopt if the address retrieval fails.
 */
template <class gsocketClass>
std::optional<Addr6> getsockname(gsocketClass *ptr, Addr6 *buff = nullptr);
