#include <zconf.h>
#include <iostream>
#include "WorkerProcess.h"
#include "../../cons/Definition.h"

using namespace std;

WorkerProcess::WorkerProcess(int fdClient) : fdClient(fdClient) {}

int WorkerProcess::live() {
    int pid = fork();
    if (pid != 0) {
        return pid;
    }
    bool connected = true;
    while (connected) {
        ClientRequest request = {};
        ssize_t res = read(fdClient, &request, sizeof(ClientRequest));
        if (res == -1) {
            throw "Fallo al intentar leer";
        } else if (res == 0) {
            connected = false;
            cout << "Cliente desconectado" << endl;
        } else {
            cout << "Pedido: " << endl
                 << "Commando: " << request.operation << endl
                 << "Consulta: " << request.query << endl;

            string response = "La respuesta es JAPON";
            write(fdClient, response.c_str(), sizeof(char) * response.size());

        }
    }
}
