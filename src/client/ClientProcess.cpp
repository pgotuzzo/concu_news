#include <zconf.h>
#include <cstdlib>
#include <iostream>
#include "ClientProcess.h"
#include "../cons/Constants.h"

using namespace std;


bool getOperationFromCommand(string &command, Operation *op) {
    bool valid = false;
    for (int i = WEATHER; i <= END; i++) {
        auto aux = static_cast<Operation> (i);
        if (MAP_COMMAND.at(aux) == command) {
            *op = aux;
            valid = true;
        }
    }
    return valid;
}

ClientProcess::ClientProcess(const char *const name, bool &isAdmin) : name(name), isAdmin(isAdmin) {}

int ClientProcess::live() {
    int pid = fork();
    if (pid != 0) {
        return pid;
    }
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
                if (request.operation == END) {
                    end = true;
                } else {
                    processRequest(request);
                }
            } else {
                cout << "No entendiste nada!" << endl << "Relee las opciones y volve a intentarlo" << endl;
            }
        }
        cout << "Gracias! Vuelvas prontos" << endl;
    }
    exit(0);
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
         << "A continuacion un breve resumen de las opciones disponibles:" << endl;
    for (int i = WEATHER; i <= END; i++) {
        auto op = static_cast<Operation> (i);
        cout << "\t" << MAP_COMMAND.at(op) << ":\t" << MAP_COMMAND_EXPLANATION.at(op) << endl;
    }
    cout << "================================================================<<" << endl;
}

bool ClientProcess::readInput(ClientRequest *request) {
    string line;
    getline(cin, line);
    char command[255];
    sscanf(line.c_str(), "%s %s %s", command, request->query, request->value);
    Operation op;
    bool valid = false;
    string sCommand(command);
    if (getOperationFromCommand(sCommand, &op)) {
        request->operation = op;
        valid = true;
    }
    return valid;
}

void ClientProcess::processRequest(ClientRequest request) {
    cout << "Procesando pedido..." << endl;
    string msg = "Soy un pedido";
    socket->send(msg.c_str(), sizeof(char) * msg.size());
    char response[255] = "";
    socket->receive(response, sizeof(char) * 255);
    cout << response << endl;
}