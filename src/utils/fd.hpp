#pragma once
#include <sys/ioctl.h>
#include "../core/__base_socket.hpp"
/**
 * @brief Gets the amount of available bytes in a file_descriptor.
 * @param fd The file descriptor of the file_descriptor.
 * @return The number of available bytes.
 */
inline constexpr auto get_available_bytes = [](int fd, int *n) {
  ioctl(fd, FIONREAD, n);
};
int get_available_bytes2(gsocket::__base_socket *sockptr);
