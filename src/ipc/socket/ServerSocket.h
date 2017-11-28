#ifndef CONCU_NEWS_SERVERSOCKET_H
#define CONCU_NEWS_SERVERSOCKET_H

#include <netinet/in.h>
#include "exception/SocketCreationException.h"
#include "exception/SocketConnectException.h"

using namespace std;

class ServerSocket {
private:
    static const char *const TAG;
    static const int MAX_CONNECTIONS = 10;

    int fdSocket;
    struct sockaddr_in servAddr;

public:
    ServerSocket(unsigned int port) throw(SocketCreationException);

    int getConnection() throw(SocketConnectException);

    void close();
};


#endif //CONCU_NEWS_SERVERSOCKET_H