#include <zconf.h>
#include <cstdlib>
#include <iostream>
#include "ServerProcess.h"
#include "../ipc/socket/ServerSocket.h"
#include "../cons/Constants.h"
#include "worker/WorkerProcess.h"

using namespace std;

ServerProcess::ServerProcess() = default;

int ServerProcess::live() {
    int pid = fork();
    if (pid != 0) {
        return pid;
    }
    cout << "Server inicializado" << endl;
    try {
        ServerSocket socket(SERVER_PORT);
        while (true) {
            int fdClient = socket.getConnection();
            cout << "Cliente conectado. Se le asigna un worker" << endl;
            WorkerProcess worker(fdClient);
            worker.live();
        }
    } catch (SocketCreationException &e) {
        cout << "Algo salio mal a la hora de CREAR el socket...LOGS" << endl;
        cout << e.what() << endl;
    } catch (SocketConnectException &e) {
        cout << "Algo salio mal a la hora de CONECTARSE al cliente...LOGS" << endl;
        cout << e.what() << endl;
    }
    exit(0);
}
