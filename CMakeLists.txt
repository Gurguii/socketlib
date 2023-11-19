cmake_minimum_required (VERSION 3.10)

project (gsocket VERSION 1.0.0 DESCRIPTION "Gurgui socket library")

include(GNUInstallDirs) # necessary to use $CMAKE_INSTALL_INCLUDEDIR

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")

set(gsocket_core
  "src/core/__base_socket.cpp"
  "src/core/implementations/await.cpp"
  "src/core/implementations/duplicate.cpp"
  "src/core/implementations/getaddress.cpp"
  "src/core/implementations/recv.cpp"
  "src/core/implementations/send.cpp"
)

set(gsocket_socket_classes
  "src/sockets/tcp/tcp4socket.cpp"
  "src/sockets/tcp/tcpsocket.cpp"
  "src/utils/net.cpp"
  "src/utils/fd.cpp"
  "src/sockets/udp/udpsocket.cpp"
  "src/sockets/udp/udp4socket.cpp"
  "src/sockets/local/localsocket.cpp"
  "src/monitoring/socket_monitor.cpp"
  "src/pipe/pipe.cpp"
  "src/pipe/namedpipe.cpp"
)

set(gsocket_testing
  "testing/bufferclass.cpp"
)

add_library(${PROJECT_NAME} SHARED
  "${gsocket_core}"
  "${gsocket_socket_classes}"
)

target_include_directories(${PROJECT_NAME} PUBLIC src)

# link another library
target_link_libraries(${PROJECT_NAME} PUBLIC /usr/lib/libuuid.so)

set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  LINKER_LANGUAGE CXX
)

install (TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
)

install(DIRECTORY src/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src
  PATTERN *.hpp
  PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

install(FILES ${gsocket_testing} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/testing/)
install(FILES gsocket.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
file(WRITE ${CMAKE_BINARY_DIR}/gsocket_root_include_dir.txt "gsocket_root_include_dir=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}\n")
