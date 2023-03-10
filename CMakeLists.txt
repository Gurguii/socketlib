# Minimum version
cmake_minimum_required (VERSION 3.9)

# Project info
project (gsocket VERSION 1.0.0 DESCRIPTION "POSIX socket wrapper")

# Grant access to standard variables related with installation directories (CMAKE_ISNTALL_xDIR)
include(GNUInstallDirs)

# Set of headers to install
set(GSOCKET_MISC
	"src/enums.hh"
	"src/socket.hh"
	"src/utils.hh"
	
	"src/socket"
	"src/utils"

	"templates/templates"
)
set (GSOCKET_CORE
	"src/core/socket_wrapper.hh"
	"src/core/socket_wrapper"
)
set (GSOCKET_WRAPPED_POSIX_FUNCTIONS
	"src/posix_wrapped_functions/connect"
	"src/posix_wrapped_functions/send"
	"src/posix_wrapped_functions/sendto"
	"src/posix_wrapped_functions/recv"
	"src/posix_wrapped_functions/recvfrom"
	"src/posix_wrapped_functions/bind"
	"src/posix_wrapped_functions/await"
	"src/posix_wrapped_functions/accept"
	"src/posix_wrapped_functions/close"
	"src/posix_wrapped_functions/listen"
	"src/posix_wrapped_functions/getpeername"
	"src/posix_wrapped_functions/getsockname"
)
set (GSOCKET_TCP_CLASSES
	"tcp_classes/tcpClient.hh"
	"tcp_classes/tcpServer.hh"

	"tcp_classes/tcpClient"
	"tcp_classes/tcpServer"
)
set (GSOCKET_UDP_CLASSES
	"udp_classes/udpClient.hh"
	"udp_classes/udpServer.hh"

	"udp_classes/udpClient"
	"udp_classes/udpServer"
)
# Declare the library target
add_library(${PROJECT_NAME} SHARED
	"${GSOCKET_MISC}"
	"${GSOCKET_CORE}"
	"${GSOCKET_WRAPPED_POSIX_FUNCTIONS}"
	"${GSOCKET_TCP_CLASSES}"
	"${GSOCKET_UDP_CLASSES}"
)

# Directories to look for header files
target_include_directories(${PROJECT_NAME} PRIVATE src tcp_classes udp_classes src/core)

# Set version property
set_target_properties(${PROJECT_NAME} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	LINKER_LANGUAGE CXX
)

# Set install destinations
install (FILES ${GSOCKET_WRAPPED_POSIX_FUNCTIONS}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src/posix_wrapped_functions
)

install (FILES ${GSOCKET_CORE}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src/core
)

install (FILES ${GSOCKET_TCP_CLASSES}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/tcp_classes
)

install (FILES ${GSOCKET_UDP_CLASSES}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/udp_classes
)

install (FILES ${GSOCKET_MISC}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src
)