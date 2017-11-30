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
#include <sys/wait.h>
#include "../ipc/queue/Queue.h"

using namespace std;

ServerProcess::ServerProcess() = default;

int ServerProcess::live() {
    int pid = fork();
    if (pid != 0) {
        return pid;
    }
    // Microservicios
    int MICROSERVICES_COUNT = 2;
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
    int counter = 0;
    // TODO: reemplazar esto con un signal handle y un graceful quit
    int CONEXIONES_HASTA_PARAR = 1;
    try {
        ServerSocket socket(SERVER_PORT);
        while (counter < CONEXIONES_HASTA_PARAR) {
            int fdClient = socket.getConnection();
            cout << "Cliente conectado. Se le asigna un worker" << endl;
            WorkerProcess worker(fdClient);
            worker.live();
            counter++;
        }
    } catch (SocketCreationException &e) {
        cout << "Algo salio mal a la hora de CREAR el socket...LOGS" << endl;
        cout << e.what() << endl;
    } catch (SocketConnectException &e) {
        cout << "Algo salio mal a la hora de CONECTARSE al cliente...LOGS" << endl;
        cout << e.what() << endl;
    }

    for(int i = 0; i < CONEXIONES_HASTA_PARAR; i++) {
        wait(NULL);
    }
    // Cola para comunicarse con los microservicios
    Queue<MicroserviceMessage> queue(PATH_QUEUE);
    MicroserviceMessage microserviceRequest{};
    microserviceRequest.requesterIdentifier = -1;
    microserviceRequest.type = CURRENCY;
    queue.send(microserviceRequest);
    microserviceRequest.type = WEATHER;
    queue.send(microserviceRequest);

    for(int i = 0; i < MICROSERVICES_COUNT; i++) {
        wait(NULL);
    }
    queue.destroy();

    exit(0);
}
