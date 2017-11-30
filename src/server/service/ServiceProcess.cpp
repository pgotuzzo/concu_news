#include <fcntl.h>
#include <fstream>
#include <zconf.h>
#include <cstring>
#include "ServiceProcess.h"
#include "../../util/FileUtils.h"
#include "../../cons/Definition.h"
#include "../../cons/Path.h"
#include "../../ipc/queue/Queue.h"

ServiceProcess::ServiceProcess(const char *const path_db, const char *const path_queue,
                               const long msType): path_db(path_db), path_queue(path_queue),
                                                   msType(msType) {}

int ServiceProcess::live() {
    int pid = fork();
    if (pid != 0) {
        return pid;
    }
    cout << "Se inicializo el microservicio " << msType << endl;
    Queue<MicroserviceRequest> queue(PATH_QUEUE);
    loadInfo();

    MicroserviceRequest microserviceRequest{};
    queue.receive(&microserviceRequest, msType);
    cout << "Se recibio un pedido del worker "  << microserviceRequest.requesterIdentifier
         << " para el microservicio " << msType << endl;
    while(microserviceRequest.requesterIdentifier != -1) {
        strcpy(microserviceRequest.value, "value");
        microserviceRequest.type = microserviceRequest.requesterIdentifier;
        queue.send(microserviceRequest);
        cout << "Se respondio un pedido al worker "  << microserviceRequest.requesterIdentifier
             << " desde el microservicio " << msType << endl;

        queue.receive(&microserviceRequest, msType);
        cout << "Se recibio un pedido del worker "  << microserviceRequest.requesterIdentifier
             << " para el microservicio " << msType << endl;
    }
    cout << "Finalizando MS" << endl;

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
