#include <sys/socket.h>
#include <sstream>
#include <netinet/in.h>
#include "ServerSocket.h"
#include "../../util/Logger.h"
#include <unistd.h>

using namespace std;

const char *const ServerSocket::TAG = "ServerSocket";

ServerSocket::ServerSocket(const unsigned int port) throw(SocketCreationException) {
    // Creates a stream socket using ipv4 and the default protocol
    fdSocket = socket(AF_INET, SOCK_STREAM, 0);
    stringstream errorMsg;
    if (fdSocket == -1) {
        errorMsg = stringstream();
        errorMsg << "Fallo la CREACION del socket! Error: " << strerror(errno);
        Logger::e(TAG, errorMsg.str());
        throw SocketCreationException(errorMsg.str());
    }
    // Init Server Address struct
    bzero(&servAddr, sizeof(sockaddr_in));
    servAddr.sin_family = AF_INET;          // ipv4
    servAddr.sin_addr.s_addr = INADDR_ANY;  // any available ip
    servAddr.sin_port = htons(port);        // port received by parameter
    // Bind
    int res = ::bind(fdSocket, (sockaddr *) &servAddr, sizeof(servAddr));
    if (res == -1) {
        errorMsg = stringstream();
        errorMsg << "Fallo el BIND del socket! Error: " << strerror(errno);
        Logger::e(TAG, errorMsg.str());
        throw SocketCreationException(errorMsg.str());
    }
    // Listen
    res = listen(fdSocket, MAX_CONNECTIONS);
    if (res == -1) {
        errorMsg = stringstream();
        errorMsg << "Fallo el LISTEN del socket! Error: " << strerror(errno);
        Logger::e(TAG, errorMsg.str());
        throw SocketCreationException(errorMsg.str());
    }
}

int ServerSocket::getConnection() throw(SocketConnectException) {
    sockaddr sockAddr = {};
    socklen_t size = sizeof(sockaddr);
    Logger::d(TAG, "Esperando conexion...");
    int fd = accept(fdSocket, &sockAddr, &size);
    if (fd == -1) {
        stringstream errorMsg;
        errorMsg << "Fallo el ACCEPT del socket! Error: " << strerror(errno);
        Logger::e(TAG, errorMsg.str());
        throw SocketConnectException(errorMsg.str());
    }
    Logger::d(TAG, "Conexion establecida!");
    return fd;
}

void ServerSocket::close_conection() {
    close(fdSocket);
}