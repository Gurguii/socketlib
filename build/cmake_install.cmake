# Install script for directory: /home/gurgui/socketlib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/gsocket/src/posix_wrapped_functions" TYPE FILE FILES
    "/home/gurgui/socketlib/src/posix_wrapped_functions/connect"
    "/home/gurgui/socketlib/src/posix_wrapped_functions/send"
    "/home/gurgui/socketlib/src/posix_wrapped_functions/sendto"
    "/home/gurgui/socketlib/src/posix_wrapped_functions/recv"
    "/home/gurgui/socketlib/src/posix_wrapped_functions/recvfrom"
    "/home/gurgui/socketlib/src/posix_wrapped_functions/bind"
    "/home/gurgui/socketlib/src/posix_wrapped_functions/await"
    "/home/gurgui/socketlib/src/posix_wrapped_functions/accept"
    "/home/gurgui/socketlib/src/posix_wrapped_functions/close"
    "/home/gurgui/socketlib/src/posix_wrapped_functions/listen"
    "/home/gurgui/socketlib/src/posix_wrapped_functions/getpeername"
    "/home/gurgui/socketlib/src/posix_wrapped_functions/getsockname"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/gsocket/src/core" TYPE FILE FILES
    "/home/gurgui/socketlib/src/core/socket_wrapper.hh"
    "/home/gurgui/socketlib/src/core/socket_wrapper"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/gsocket/tcp_classes" TYPE FILE FILES
    "/home/gurgui/socketlib/tcp_classes/tcpClient.hh"
    "/home/gurgui/socketlib/tcp_classes/tcpServer.hh"
    "/home/gurgui/socketlib/tcp_classes/tcpClient"
    "/home/gurgui/socketlib/tcp_classes/tcpServer"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/gsocket/udp_classes" TYPE FILE FILES
    "/home/gurgui/socketlib/udp_classes/udpClient.hh"
    "/home/gurgui/socketlib/udp_classes/udpServer.hh"
    "/home/gurgui/socketlib/udp_classes/udpClient"
    "/home/gurgui/socketlib/udp_classes/udpServer"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/gsocket/src" TYPE FILE FILES
    "/home/gurgui/socketlib/src/enums.hh"
    "/home/gurgui/socketlib/src/socket.hh"
    "/home/gurgui/socketlib/src/utils.hh"
    "/home/gurgui/socketlib/src/socket"
    "/home/gurgui/socketlib/src/utils"
    "/home/gurgui/socketlib/templates/templates"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/gurgui/socketlib/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
