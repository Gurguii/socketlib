#pragma once

/* Main header file from `libgsocket`.
* Imports everything in the lib */

#include "src/core/__base_socket.hpp"

#include "src/sockets/tcp/tcpsocket.hpp"
#include "src/sockets/tcp/tcp4socket.hpp"

#include "src/sockets/udp/udpsocket.hpp"
#include "src/sockets/udp/udp4socket.hpp"

#include "src/sockets/local/localsocket.hpp"

#include "src/monitoring//socket_monitor.hpp"

#include "src/pipe/pipe.hpp"
#include "src/pipe/namedpipe.hpp"

#include "src/utils/fd.hpp"
#include "src/utils/net.hpp"

#include "src/defines.hpp"
#include "src/structs.hpp"
#include "src/enums.hpp"
