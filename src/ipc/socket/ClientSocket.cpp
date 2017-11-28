#include <sys/socket.h>
#include <cstring>
#include <string>
#include <sstream>
#include <netinet/in.h>
#include <zconf.h>
#include <netdb.h>
#include "ClientSocket.h"
#include "../../util/Logger.h"

using namespace std;

const char *const ClientSocket::TAG = "ClientSocket";

ClientSocket::ClientSocket(const char *serverIp, const unsigned int port) throw(SocketCreationException) :
        serverIp(serverIp), serverPort(port) {
    // Creates a stream socket using ipv4 and the default protocol
    fdSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (fdSocket == -1) {
        stringstream errorMsg;
        errorMsg << "Fallo la creacion del socket! Error: " << strerror(errno);
        Logger::e(TAG, errorMsg.str());
        throw SocketCreationException(errorMsg.str());
    }
    // Init Server Address struct
    bzero(&servAddr, sizeof(sockaddr_in));
    servAddr.sin_family = AF_INET;          // ipv4
    servAddr.sin_port = htons(serverPort);  // server port
}

void ClientSocket::openConnection() throw(SocketConnectException) {
    Logger::d(TAG, "Estableciendo conexion...");

    struct hostent *server = gethostbyname(serverIp);
    if (server == nullptr) {
        stringstream errorMsg;
        errorMsg << "No se puede localizar el host! Error " << errno << ": " << strerror(errno);
        Logger::e(TAG, errorMsg.str());
        throw SocketConnectException(errorMsg.str());
    }
    bcopy(server->h_addr, (char *) &(this->servAddr.sin_addr.s_addr), server->h_length);

    int res = connect(fdSocket, (sockaddr *) &servAddr, sizeof(servAddr));
    if (res == -1) {
        stringstream errorMsg;
        errorMsg << "Fallo la conexion con el server! Error " << errno << ": " << strerror(errno);
        Logger::e(TAG, errorMsg.str());
        throw SocketConnectException(errorMsg.str());
    }
    Logger::d(TAG, "Conexion establecida!");
}

ssize_t ClientSocket::send(const void *buffer, size_t buffSize) {
    return write(fdSocket, buffer, buffSize);
}

ssize_t ClientSocket::receive(void *buffer, size_t buffSize) {
    return read(fdSocket, buffer, buffSize);
}

void ClientSocket::closeConnection() {
    close(fdSocket);
    Logger::d(TAG, "La conexion fue cerrada correctamente");
}