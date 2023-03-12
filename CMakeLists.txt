# Minimum version
cmake_minimum_required (VERSION 3.9)

# Project info
project (gsocket VERSION 1.0.0 DESCRIPTION "POSIX socket wrapper")

# Grant access to standard variables related with installation directories (CMAKE_ISNTALL_xDIR)
include(GNUInstallDirs)

set(GSOCKET_CORE
	"src/core/socket_wrapper.hh"
	"src/core/socket_wrapper"
)

set(GSOCKET_POSIX_WRAPPED_FUNCTIONS
	"src/posix_wrapped_functions/accept"
	"src/posix_wrapped_functions/await"
	"src/posix_wrapped_functions/bind"
	"src/posix_wrapped_functions/close"
	"src/posix_wrapped_functions/connect"
	"src/posix_wrapped_functions/listen"
	"src/posix_wrapped_functions/recv"
	"src/posix_wrapped_functions/recvfrom"
	"src/posix_wrapped_functions/send"
	"src/posix_wrapped_functions/sendto"
	"src/posix_wrapped_functions/getpeername"
	"src/posix_wrapped_functions/getsockname"
)

set(GSOCKET_SOCKET
	"src/socket.hh"
	"src/socket"
)

set(GSOCKET_MISC
	"src/utils.hh"
	"src/utils"
	"src/enums.hh"
	"templates/templates"
)

set(GSOCKET_TCP_CLASSES
	"tcp_classes/tcpClient"
	"tcp_classes/tcpServer"
	"tcp_classes/tcpClient.hh"
	"tcp_classes/tcpServer.hh"
)

set(GSOCKET_UDP_CLASSES
	"udp_classes/udpClient"
	"udp_classes/udpServer"
	"udp_classes/udpClient.hh"
	"udp_classes/udpServer.hh"
)
# Declare the library target
add_library(${PROJECT_NAME} SHARED
	"${GSOCKET_CORE}"
	"${GSOCKET_POSIX_WRAPPED_FUNCTIONS}"
	"${GSOCKET_SOCKET}"
	"${GSOCKET_TCP_CLASSES}"
	"${GSOCKET_UDP_CLASSES}"
	"${GSOCKET_MISC}"
)

# Directories to look for header files
target_include_directories(${PROJECT_NAME} PRIVATE src tcp_classes udp_classes)

# Set version property
set_target_properties(${PROJECT_NAME} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	LINKER_LANGUAGE CXX
)

# Install library
install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install files
install(DIRECTORY src/core src/posix_wrapped_functions DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src)
install(FILES src/socket DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES src/socket.hh DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src)
install(DIRECTORY tcp_classes DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/)
install(DIRECTORY udp_classes DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/)
install(FILES src/enums.hh src/utils.hh src/utils templates/templates DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/misc)