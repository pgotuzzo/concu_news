#include <zconf.h>
#include <cstdlib>
#include <iostream>
#include "ServerProcess.h"
#include "../ipc/socket/ServerSocket.h"
#include "../cons/Constants.h"

using namespace std;

ServerProcess::ServerProcess() = default;

int ServerProcess::live() {
    int pid = fork();
    if (pid != 0) {
        return pid;
    }
    // FIXME - Implement REAL logic
    cout << "Server inicializado" << endl;
    try {
        ServerSocket socket(SERVER_PORT);
        int fdClient = socket.getConnection();
        cout << "Cliente conectado" << endl;
        bool connected = true;
        while (connected) {
            char buffer[255];
            ssize_t res = read(fdClient, buffer, sizeof(char) * 255);
            if (res == -1) {
                throw "Fallo al intentar leer";
            } else if (res == 0) {
                connected = false;
                cout << "Cliente desconectado" << endl;
            } else {
                cout << "Pedido: " << buffer << endl;

                string response = "La respuesta es JAPON";
                write(fdClient, response.c_str(), sizeof(char) * response.size());

            }
        }
    } catch (...) {
        cout << "Algo salio mal...LOGS" << endl;
    }

    exit(0);
}
