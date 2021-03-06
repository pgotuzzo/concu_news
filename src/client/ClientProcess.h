#ifndef CONCU_NEWS_CLIENTPROCESS_H
#define CONCU_NEWS_CLIENTPROCESS_H


#include "../ipc/socket/ClientSocket.h"
#include "../cons/Definition.h"

class ClientProcess {
private:
    static const int RETRY_LIMIT = 3;
    static const int RETRY_FREQUENCY_SEC = 5;

    bool isAdmin;
    const char *const name;
    ClientSocket *socket;

    ClientSocket *connectWithServer();

    void welcome();

    bool readInput(ClientRequest *request);

    bool processRequest(ClientRequest request);

    bool getOperationFromCommand(string command_service, const string &command_operation, ServiceOperation *sop);
public:
    ClientProcess(const char *name, bool &isAdmin);

    int live();
};


#endif //CONCU_NEWS_CLIENTPROCESS_H
