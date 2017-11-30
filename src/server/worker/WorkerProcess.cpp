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
    Queue<MicroserviceMessage> queue(PATH_QUEUE);
    MicroserviceMessage microserviceMessage{};
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
                 << "Operacion: " << request.serviceOperation.operation << endl
                 << "Servicio: " << request.serviceOperation.service << endl
                 << "Consulta: " << request.query << endl;

            connected = request.serviceOperation.operation != END;

            if (connected) {
                strcpy(microserviceMessage.query, request.query);
                strcpy(microserviceMessage.value, request.value);
                microserviceMessage.requesterIdentifier = workerIdentifier;
                microserviceMessage.type = request.serviceOperation.service;
                microserviceMessage.operation = request.serviceOperation.operation;

                queue.send(microserviceMessage);

                cout << "Se envio el pedido al microservicio con el identificador "<< workerIdentifier << endl;
                queue.receive(&microserviceMessage, workerIdentifier);
                cout << "Se recibio la respuesta del microservicio para el identificador "<< workerIdentifier << endl;

                string response = string(microserviceMessage.value);
                write(fdClient, response.c_str(), sizeof(char) * response.size());
            }
        }
    }
    cout << "Se termino la conexion del worker con identificador "<< workerIdentifier << endl;
}
