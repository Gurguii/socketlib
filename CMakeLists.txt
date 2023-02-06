# Minimum version
cmake_minimum_required (VERSION 3.9)

# Project info
project (gsocket VERSION 1.0.0 DESCRIPTION "POSIX socket wrapper")

# Grant access to standard variables related with installation directories (CMAKE_ISNTALL_xDIR)
include(GNUInstallDirs)

# Set of headers to install
set(GSOCKET_PUBLIC_HEADERS
	"src/enums.hh"
	"src/socket_wrapper.hh"
	"src/socket.hh"
	"src/utils.hh"
	"tcp_classes/tcpClient.hh"
	"tcp_classes/tcpServer.hh"
	"udp_classes/udpClient.hh"
	"udp_classes/udpServer.hh"
	
	"src/socket_wrapper"
	"src/socket"
	"src/utils"
	"tcp_classes/tcpClient"
	"tcp_classes/tcpServer"
	"udp_classes/udpClient"
	"udp_classes/udpServer"

	"templates/templates"
)

# Declare the library target
add_library(${PROJECT_NAME} SHARED
	"${GSOCKET_PUBLIC_HEADERS}"
)

# Directories to look for header files
target_include_directories(${PROJECT_NAME} PRIVATE src tcp_classes udp_classes)

# Set version property
set_target_properties(${PROJECT_NAME} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	LINKER_LANGUAGE CXX
	PUBLIC_HEADER "${GSOCKET_PUBLIC_HEADERS}"
)

# Set install destinations
install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
