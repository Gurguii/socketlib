# Minimum version
cmake_minimum_required (VERSION 3.10)

# Project info
project (gsocket VERSION 1.0.0 DESCRIPTION "POSIX socket wrapper")

# Grant access to standard variables related to installation directories (CMAKE_INSTALL_xDIR)
include(GNUInstallDirs)

# Set g++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -O3 -Wall")

# Set the source files for each component of the library
set(gsocket_constructors
  "src/core/socket_wrapper.cpp"
)

set(gsocket_posix_wrapped_functions
	"src/posix_wrapped_functions/accept.cpp"
	"src/posix_wrapped_functions/await.cpp"
	"src/posix_wrapped_functions/bind.cpp"
	"src/posix_wrapped_functions/close.cpp"
	"src/posix_wrapped_functions/connect.cpp"
	"src/posix_wrapped_functions/listen.cpp"
	"src/posix_wrapped_functions/recv.cpp"
	"src/posix_wrapped_functions/recvfrom.cpp"
	"src/posix_wrapped_functions/send.cpp"
	"src/posix_wrapped_functions/sendto.cpp"
	"src/posix_wrapped_functions/getpeername.cpp"
	"src/posix_wrapped_functions/getsockname.cpp"
)

set (gsocket_classes
	"src/socket.cpp"
	"src/classes/pipe/pipe.cpp"
	"src/classes/tcp/tcpServer.cpp"
	"src/classes/tcp/tcpClient.cpp"
)

set(gsocket_utils
	"src/utils/net.cpp"
)
set(gsocket_templates
	"templates/templates.cpp"
)

# Declare the library target // .cpp files (implementations) to compile
add_library(${PROJECT_NAME} SHARED
  "${gsocket_constructors}"
  "${gsocket_posix_wrapped_functions}"
  "${gsocket_utils}"
  "${gsocket_templates}"
  "${gsocket_classes}"
)

# Directories to look for header files
target_include_directories(${PROJECT_NAME} PRIVATE src templates)

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

# Install header files and other directories
# gsocket base class
install(FILES src/core/socket_wrapper.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src/core/)
install(FILES src/enums.hpp src/socket.hpp src/structs.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src/)
# gsocket utils 
install(FILES src/utils/net.hpp src/utils/fd.hpp src/utils/socket.hpp src/utils/socket_generator.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src/utils/)
# gsocket classes
install(FILES src/classes/pipe/pipe.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src/classes/pipe/)
install(FILES src/classes/tcp/tcpClient.hpp src/classes/tcp/tcpServer.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src/classes/tcp/)
# gsocket templates
install(FILES templates/templates.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/templates/)
# gsocket main header 
install(FILES gsocket.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/)
