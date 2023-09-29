#pragma once
#include <sys/ioctl.h>
namespace gsocket
{

/**
 * @brief Gets the amount of available bytes in a file_descriptor.
 * @param fd The file descriptor of the file_descriptor.
 * @return The number of available bytes.
 */
inline constexpr auto availableBytes = [](int fd, int *n) {
  ioctl(fd, FIONREAD, n);
};
}
