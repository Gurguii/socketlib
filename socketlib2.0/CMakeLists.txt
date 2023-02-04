# Minimum version
cmake_minimum_required (VERSION 3.9)
# Project info
project (gsocket VERSION 1.0.0 DESCRIPTION "POSIX socket wrapper")
# Declare the library target
add_library(${PROJECT_NAME} SHARED
	src/socket_wrapper.cpp
	src/socket_wrapper.hh
	src/socket.cpp
	src/socket.hh
	src/utils.cpp
	src/utils.hh
	src/enums.hh

	tcp_classes/tcpClient.cpp
	tcp_classes/tcpClient.hh
	tcp_classes/tcpServer.cpp
	tcp_classes/tcpServer.hh

	udp_classes/udpClient.cpp
	udp_classes/udpClient.hh
	udp_classes/udpServer.cpp
	udp_classes/udpServer.hh

	prototype/test.hh
)
# Directories to look for header files
target_include_directories(${PROJECT_NAME} PRIVATE src tcp_classes udp_classes)
# Set version property
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
# Set the shared object version property to the project's major version.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
# Set the public header property
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER 
	src/socket_wrapper.hh
	src/socket.hh
	src/utils.hh
	src/enums.hh
	
	tcp_classes/tcpClient.hh
	tcp_classes/tcpServer.hh
	
	udp_classes/udpClient.hh
	udp_classes/udpServer.hh
	
	protype/test.hh
)