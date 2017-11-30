#include <zconf.h>
#include <iostream>
#include "WorkerProcess.h"
#include "../../cons/Definition.h"
#include "../../cons/Path.h"
#include "../../ipc/queue/Queue.h"

using namespace std;

WorkerProcess::WorkerProcess(int fdClient) : fdClient(fdClient) {}

int WorkerProcess::live() {
    int pid = fork();
    if (pid != 0) {
        return pid;
    }


    // Cola para comunicarse con los microservicios
    Queue<MicroserviceRequest> queue(PATH_QUEUE);
    MicroserviceRequest microserviceRequest{};
    long workerIdentifier = getpid()+1000;
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

            strcpy(microserviceRequest.query, request.query);
            microserviceRequest.requesterIdentifier = workerIdentifier;
            microserviceRequest.type = request.operation;
            queue.send(microserviceRequest);

            cout << "Se envio el pedido al microservicio con el identificador"<< workerIdentifier << endl;
            queue.receive(&microserviceRequest, workerIdentifier);
            cout << "Se recibio la respuesta del microservicio para el identificador"<< workerIdentifier << endl;

            string response = string(microserviceRequest.value);
            write(fdClient, response.c_str(), sizeof(char) * response.size());

            connected = request.operation != END;
        }
    }
}
