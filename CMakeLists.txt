cmake_minimum_required(VERSION 3.8)
project(concu_news)

set(CMAKE_CXX_STANDARD 11)

set(IPC
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
        src/client/ClientProcess.cpp src/client/ClientProcess.h)

set(SERVER
        src/server/ServerProcess.cpp src/server/ServerProcess.h
        src/server/worker/WorkerProcess.cpp src/server/worker/WorkerProcess.h
        src/server/service/ServiceProcess.cpp src/server/service/ServiceProcess.h)

set(SOURCE_FILES src/main.cpp)

add_executable(concu_news ${SOURCE_FILES} ${IPC} ${CONS} ${UTILS} ${CLIENT} ${SERVER})

configure_file(help ${CMAKE_CURRENT_BINARY_DIR}/help COPYONLY)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/db)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/logs)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tmp)