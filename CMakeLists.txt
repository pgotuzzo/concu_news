cmake_minimum_required(VERSION 3.8)
project(concu_news)

set(CMAKE_CXX_STANDARD 11)

set(IPC
        src/ipc/handler/SignalHandler.cpp src/ipc/handler/SignalHandler.h
        src/ipc/handler/EventHandler.h src/ipc/handler/SIGTERM_SIGINT_QUIT_Handler.h
        src/ipc/socket/ClientSocket.cpp src/ipc/socket/ClientSocket.h
        src/ipc/socket/ServerSocket.cpp src/ipc/socket/ServerSocket.h
        src/ipc/socket/exception/SocketCreationException.cpp src/ipc/socket/exception/SocketCreationException.h
        src/ipc/socket/exception/SocketConnectException.cpp src/ipc/socket/exception/SocketConnectException.h
        src/ipc/queue/Queue.h
        src/ipc/queue/exception/QueueSndRcvException.cpp src/ipc/queue/exception/QueueSndRcvException.h
        src/ipc/queue/exception/QueueDestroyException.cpp src/ipc/queue/exception/QueueDestroyException.h
        src/ipc/queue/exception/QueueCreationException.cpp src/ipc/queue/exception/QueueCreationException.h)

set(CONS src/cons/Constants.h src/cons/Definition.h src/cons/Path.h)

set(UTILS
        src/util/FileUtils.cpp src/util/FileUtils.h
        src/util/Logger.cpp src/util/Logger.h
        src/util/StringUtils.cpp src/util/StringUtils.h)

set(CLIENT
        src/client/main.cpp
        src/client/ClientProcess.cpp src/client/ClientProcess.h)

set(SERVER
        src/server/main.cpp
        src/server/ServerProcess.cpp src/server/ServerProcess.h
        src/server/worker/WorkerProcess.cpp src/server/worker/WorkerProcess.h
        src/server/service/ServiceProcess.cpp src/server/service/ServiceProcess.h)

add_executable(server ${SERVER} ${IPC} ${CONS} ${UTILS} )
add_executable(client ${CLIENT} ${IPC} ${CONS} ${UTILS} )

configure_file(help ${CMAKE_CURRENT_BINARY_DIR}/help COPYONLY)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/db)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/logs)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tmp)