#ifndef CONCU_NEWS_CLIENTSOCKET_H
#define CONCU_NEWS_CLIENTSOCKET_H

#include <netinet/in.h>
#include "exception/SocketCreationException.h"
#include "exception/SocketConnectException.h"

using namespace std;

class ClientSocket {
private:
    static const char *const TAG;

    int fdSocket;
    const char *serverIp;
    const int serverPort;
    struct sockaddr_in servAddr;

public:
    ClientSocket(const char *serverIp, unsigned int port) throw(SocketCreationException);

    void openConnection() throw(SocketConnectException);

    ssize_t send(const void *buffer, size_t buffSize);

    ssize_t receive(void *buffer, size_t buffSize);

    void closeConnection();
};

#endif //CONCU_NEWS_CLIENTSOCKET_H
