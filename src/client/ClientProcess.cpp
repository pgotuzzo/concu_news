#include <zconf.h>
#include <cstdlib>
#include <iostream>
#include "ClientProcess.h"
#include "../cons/Constants.h"

using namespace std;


bool ClientProcess::getOperationFromCommand(string command_service, const string &command_operation, ServiceOperation *sop) {
    if (!isAdmin) {
        auto operation = COMMAND_TO_OPERATION.find(command_service);
        bool valid = operation != COMMAND_TO_OPERATION.end();
        if (!valid) {
            auto service = COMMAND_TO_SERVICE.find(command_service);
            valid = service != COMMAND_TO_SERVICE.end();
            if (valid) {
                sop->operation = READ;
                sop->service = service->second;
            }
        } else if (operation->second == END) {
            sop->operation = operation->second;
        } else {
            valid = false;
        }
        return valid;
    }
    auto operation = COMMAND_TO_OPERATION.find(command_operation);
    bool valid = operation != COMMAND_TO_OPERATION.end();
    if (valid) {
        sop->operation = operation->second;
        if (operation->second != END) {
            auto service = COMMAND_TO_SERVICE.find(command_service);
            valid = service != COMMAND_TO_SERVICE.end();
            if (valid) {
                sop->service = service->second;
            }
        }
    }
    return valid;
}

ClientProcess::ClientProcess(const char *const name, bool &isAdmin) : name(name), isAdmin(isAdmin) {}

int ClientProcess::live() {
    socket = connectWithServer();
    if (socket == nullptr) {
        cout << "Server no disponible. Intente nuevamente mas tarde" << endl;
    } else {
        welcome();
        bool end = false;
        ClientRequest request = {};
        while (!end) {
            cout << "Que desea saber?" << endl;
            if (readInput(&request)) {
                if (request.serviceOperation.operation == END) {
                    // Sends the end operation to the server.
                    socket->send(&request, sizeof(ClientRequest));
                    end = true;
                } else {
                    end = !processRequest(request);
                }
            } else {
                cout << "No entendiste nada!" << endl << "Relee las opciones y volve a intentarlo" << endl;
            }
        }
        cout << "Gracias! Vuelvas prontos" << endl;
    }
    return 0;
}

ClientSocket *ClientProcess::connectWithServer() {
    int tryCount = 0;
    ClientSocket *socket = nullptr;
    while (tryCount < RETRY_LIMIT) {
        try {
            socket = new ClientSocket(SERVER_IP, SERVER_PORT);
            socket->openConnection();
            return socket;
        } catch (...) {
            cout << "No se pudo conectar con el server...se volvera a intentar en " << RETRY_FREQUENCY_SEC
                 << " segundos..."
                 << endl;
            sleep(RETRY_FREQUENCY_SEC);
            tryCount++;
        }
    }
    return nullptr;
}

void ClientProcess::welcome() {
    cout << "================================================================<<" << endl
         << "Bienvenido a Noticias Concu" << endl
         << "A continuacion un breve resumen de los servicios disponibles:" << endl;
    for (int i = WEATHER; i <= CURRENCY; i++) {
        auto op = static_cast<Service> (i);
        cout << "\t" << MAP_COMMAND.at(op) << ":\t" << MAP_COMMAND_EXPLANATION.at(op) << endl;
    }
    cout << "Y las operaciones posibles:" << endl;
    if (isAdmin) {
        for (int i = CREATE; i <= END; i++) {
            auto op = static_cast<Operation > (i);
            cout << "\t" << MAP_OPERATION.at(op) << ":\t" << MAP_OPERATION_EXPLANATION.at(op) << endl;
        }
    } else {
        cout << "\t" << SERVICE_END << ":\t" << MAP_OPERATION_EXPLANATION.at(END) << endl;
        cout << "\t" << OPERATION_READ << ":\t" << "permite leer datos. Se usa con [SERVICIO] [CLAVE]" << endl;
    }
    cout << "================================================================<<" << endl;
}

/**
 * TODO - Agregar soporte para usuarios admin
 * Hasta el momento solo se contemplan consultas
 */
bool ClientProcess::readInput(ClientRequest *request) {
    string line;
    getline(cin, line);
    char cmd_service[255];
    char cmd_operation[255];

    if (isAdmin) {
        sscanf(line.c_str(), "%s %s %s %s", cmd_operation, cmd_service, request->query, request->value);
    } else {
        sscanf(line.c_str(), "%s %s", cmd_service, request->query);
    }
    ServiceOperation op{};
    bool valid = false;
    string sCommand(cmd_service);
    string sOperation(cmd_operation);
    if (getOperationFromCommand(sCommand, sOperation, &op)) {
        request->serviceOperation = op;
        valid = true;
    }
    return valid;
}

bool ClientProcess::processRequest(ClientRequest request) {
    cout << "Procesando pedido..." << endl;
    ssize_t res = socket->send(&request, sizeof(ClientRequest));
    bool success = true;
    string errorMsg;
    if (res <= 0) {
        success = false;
        cout << "Se perdio conexion con el servidor. Intente nuevamente mas tarde." << endl;
    }
    char response[255] = "";
    res = socket->receive(response, sizeof(char) * 255);
    if (res <= 0) {
        success = false;
        cout << "Se perdio conexion con el servidor. Intente nuevamente mas tarde." << endl;
    } else {
        cout << response << endl;
    }
    return success;
}