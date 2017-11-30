#include <fcntl.h>
#include <fstream>
#include <zconf.h>
#include <cstring>
#include "ServiceProcess.h"
#include "../../util/FileUtils.h"
#include "../../cons/Definition.h"
#include "../../cons/Path.h"
#include "../../ipc/queue/Queue.h"
#include "../../ipc/handler/SIGTERM_SIGINT_QUIT_Handler.h"
#include "../../ipc/handler/SignalHandler.h"

ServiceProcess::ServiceProcess(const char *const path_db, const char *const path_queue,
                               const long msType): path_db(path_db), path_queue(path_queue),
                                                   msType(msType) {}

int ServiceProcess::live() {
    SIGTERM_SIGINT_QUIT_Handler sigHandler;
    SignalHandler::getInstance()->registrarHandler(SIGTERM, &sigHandler);
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigHandler);
    int pid = fork();
    if (pid != 0) {
        return pid;
    }
    cout << "Se inicializo el microservicio " << msType << endl;
    Queue<MicroserviceMessage> queue(PATH_QUEUE);
    loadInfo();

    MicroserviceMessage microserviceMessage{};
    try {
        queue.receive(&microserviceMessage, msType);
    } catch (QueueSndRcvException &e) {
        if (!sigHandler.getGracefulQuit()) {
            cout << "Algo salio mal recibiendo o respondiendo un pedido" << endl;
            cout << e.what() << endl;
        } else {
            queue.receive(&microserviceMessage, msType);
        }
    }
    cout << "Se recibio un pedido del worker "  << microserviceMessage.requesterIdentifier
         << " para el microservicio " << msType << endl;
    while(microserviceMessage.requesterIdentifier != -1) {
        microserviceMessage.type = microserviceMessage.requesterIdentifier;
        auto messageQuery = mInfo.find(microserviceMessage.query);
        switch (microserviceMessage.operation) {
            case CREATE: {
                if (messageQuery != mInfo.end()) {
                    strcpy(microserviceMessage.value, "ERROR: La clave ya existe.");
                } else {
                    mInfo[microserviceMessage.query] = microserviceMessage.value;
                    strcpy(microserviceMessage.value, "CREADO");
                }
                break;
            }
            case READ: {
                if (messageQuery != mInfo.end()) {
                    strcpy(microserviceMessage.value, messageQuery->second.c_str());
                } else {
                    strcpy(microserviceMessage.value, "ERROR: La clave no existe.");
                }
                break;
            }
            case UPDATE: {
                if (messageQuery != mInfo.end()) {
                    messageQuery->second = microserviceMessage.value;
                    strcpy(microserviceMessage.value, "EDITADO");
                } else {
                    strcpy(microserviceMessage.value, "ERROR: La clave no existe.");
                }
                break;
            }
            case DELETE: {
                if (messageQuery != mInfo.end()) {
                    mInfo.erase(messageQuery);
                    strcpy(microserviceMessage.value, "ELIMINADO");
                } else {
                    strcpy(microserviceMessage.value, "ERROR: La clave no existe.");
                }
                break;
            }
            default: {
                strcpy(microserviceMessage.value, "ERROR");
            }
        }
        try {
            queue.send(microserviceMessage);
            cout << "Se respondio un pedido al worker "  << microserviceMessage.requesterIdentifier
                 << " desde el microservicio " << msType << endl;
            queue.receive(&microserviceMessage, msType);
            cout << "Se recibio un pedido del worker "  << microserviceMessage.requesterIdentifier
                 << " para el microservicio " << msType << endl;
        } catch (QueueSndRcvException &e) {
            if (!sigHandler.getGracefulQuit()) {
                cout << "Algo salio mal recibiendo o respondiendo un pedido" << endl;
                cout << e.what() << endl;
            }
        }
    }
    cout << "Finalizando MS " << msType << endl;

    saveInfo();
    exit(0);
}

void ServiceProcess::loadInfo() {
    createFileIfNotExists(path_db);
    ifstream file(path_db);
    string line;
    char key[255];
    char value[255];
    while (std::getline(file, line)) {
        memset(key, ' ', sizeof(char) * 255);
        memset(value, ' ', sizeof(char) * 255);
        sscanf(line.c_str(), "%s %s", key, value);
        mInfo[string(key)] = string(value);
    }
    file.close();
}

void ServiceProcess::saveInfo() {
    createFileIfNotExists(path_db);
    ofstream file(path_db);
    string line;
    for (pair<string, string> entry : mInfo) {
        file << entry.first << " " << entry.second << endl;
    }
    file.close();
}
