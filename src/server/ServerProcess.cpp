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
#include "../ipc/handler/SignalHandler.h"
#include "../ipc/handler/SIGTERM_SIGINT_QUIT_Handler.h"

using namespace std;

ServerProcess::ServerProcess() = default;

int ServerProcess::live() {
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

    SIGTERM_SIGINT_QUIT_Handler sigHandler;
    SignalHandler::getInstance()->registrarHandler(SIGTERM, &sigHandler);
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigHandler);

    vector<pid_t > workers;
    // Conexiones
    cout << "Esperando conexiones..." << endl;
    try {
        ServerSocket socket(SERVER_PORT);
        while (!sigHandler.getGracefulQuit()) {
            int fdClient = socket.getConnection();
            cout << "Cliente conectado. Se le asigna un worker" << endl;
            WorkerProcess worker(fdClient, &sigHandler);
            workers.push_back(worker.live());
        }
        socket.close_conection();
    } catch (SocketCreationException &e) {
        cout << "Algo salio mal a la hora de CREAR el socket...LOGS" << endl;
        cout << e.what() << endl;
    } catch (SocketConnectException &e) {
        if (!sigHandler.getGracefulQuit()) {
            cout << "Algo salio mal a la hora de CONECTARSE al cliente...LOGS" << endl;
            cout << e.what() << endl;
        }
    }

    for(pid_t worker:workers) {
        cout << "esperando " <<worker<< endl;
        waitpid(worker, NULL, 0);
        cout << "listo " <<worker<< endl;
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
    cout << "Se apago el servidor." << endl;

    return 0;
}
