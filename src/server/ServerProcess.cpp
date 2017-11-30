#include <zconf.h>
#include <cstdlib>
#include <iostream>
#include "ServerProcess.h"
#include "../ipc/socket/ServerSocket.h"
#include "../cons/Constants.h"
#include "worker/WorkerProcess.h"
#include "service/ServiceProcess.h"
#include "../cons/Path.h"
#include "../cons/Definition.h"
#include "../ipc/queue/Queue.h"

using namespace std;

ServerProcess::ServerProcess() = default;

int ServerProcess::live() {
    int pid = fork();
    if (pid != 0) {
        return pid;
    }
    // Microservicios
    cout << "Server inicializando..." << endl;
    cout << "Inicializando microservicio para informes de TEMPERATURAS" << endl;
    ServiceProcess tempService(PATH_DB_TEMPERATURES, PATH_QUEUE, WEATHER);
    tempService.live();
    cout << "Inicializando microservicio para informes de CAMBIO DE MONEDA" << endl;
    ServiceProcess currencyService(PATH_DB_CURRENCY, PATH_QUEUE, CURRENCY);
    currencyService.live();
    cout << "Server inicializado con exito!" << endl;

    // Conexiones
    cout << "Esperando conexiones..." << endl;
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
