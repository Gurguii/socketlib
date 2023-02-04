# Minimum version
cmake_minimum_required (VERSION 3.9)
# Project info
project (gsocket VERSION 1.0.0 DESCRIPTION "POSIX socket wrapper")
#set(ENV {testingvariable} "gurguitotesting")
#execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/setup.sh")
# Set of headers to install
set(GSOCKET_PUBLIC_HEADERS
	"src/enums.hh"
	"src/socket_wrapper.hh"
	"src/gsocket.hh"
	"src/utils.hh"
	"tcp_classes/tcpClient.hh"
	"tcp_classes/tcpServer.hh"
	"udp_classes/udpClient.hh"
	"udp_classes/udpServer.hh"
	
	"src/socket_wrapper"
	"src/gsocket"
	"src/utils"
	"tcp_classes/tcpClient"
	"tcp_classes/tcpServer"
	"udp_classes/udpClient"
	"udp_classes/udpServer"

	"templates/gsocket_templates"
)
# Declare the library target
add_library(${PROJECT_NAME} SHARED
	src/socket_wrapper
	src/socket_wrapper.hh
	src/gsocket
	src/gsocket.hh
	src/utils
	src/utils.hh
	src/enums.hh

	tcp_classes/tcpClient
	tcp_classes/tcpClient.hh
	tcp_classes/tcpServer
	tcp_classes/tcpServer.hh

	udp_classes/udpClient
	udp_classes/udpClient.hh
	udp_classes/udpServer
	udp_classes/udpServer.hh

	templates/gsocket_templates
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
# Grant access to standard variables related with installation directories (CMAKE_ISNTALL_xDIR)
include(GNUInstallDirs)

# Ser lib*.so and API header to install
install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})