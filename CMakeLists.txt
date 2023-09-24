# Minimum version
cmake_minimum_required (VERSION 3.9)

# Project info
project (gsocket VERSION 1.0.0 DESCRIPTION "POSIX socket wrapper")

# Grant access to standard variables related to installation directories (CMAKE_INSTALL_xDIR)
include(GNUInstallDirs)

# Set the source files for each component of the library
set(GSOCKET_CONSTRUCTORS
  "src/core/socket_wrapper.cpp"
)

set(GSOCKET_POSIX_WRAPPED_FUNCTIONS
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

set(GSOCKET_CLASS_SOCKET
  "src/socket.cpp"
)
set(GSOCKET_UTILS
	"src/utils.cpp"
)
set(GSOCKET_TEMPLATES
	"templates/templates.cpp"
)

# Declare the library target // .cpp files (implementations) to compile
add_library(${PROJECT_NAME} SHARED
  "${GSOCKET_CONSTRUCTORS}"
  "${GSOCKET_POSIX_WRAPPED_FUNCTIONS}"
  "${GSOCKET_CLASS_SOCKET}"
  "${GSOCKET_UTILS}"
  "${GSOCKET_TEMPLATES}"
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
install(FILES src/enums.hh src/socket.hh src/utils.hh DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src/)
install(FILES src/core/socket_wrapper.hh DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/src/core/)
install(FILES templates/templates.hh DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/templates/)
install(FILES gsocket.hh DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/)

# Write installation paths to scripts/.installpaths
file(
	WRITE "scripts/.installpaths" "libdir=(${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/lib${PROJECT_NAME}.so 
	${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/lib${PROJECT_NAME}.so.1 
	${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/lib${PROJECT_NAME}.so.1.0.0)\n"
)
file(APPEND "scripts/.installpaths" "includedir=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}\n")
file(APPEND "scripts/.installpaths" "builddir=${CMAKE_BINARY_DIR}\n")
