#ifndef CONCU_NEWS_DEFINITION_H
#define CONCU_NEWS_DEFINITION_H

#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include "Constants.h"

using namespace std;

enum Service {
    WEATHER = 7,
    CURRENCY = 8
};

enum Operation {
    CREATE = 2,
    READ = 3,
    UPDATE = 4,
    DELETE = 5,
    END = 6,
};

struct ServiceOperation {
    Service service;
    Operation operation;
};

const map<Service, string> MAP_COMMAND = {
        {WEATHER,  SERVICE_WEATHER},
        {CURRENCY, SERVICE_CURRENCY}
};

const map<string, Service> COMMAND_TO_SERVICE = {
        {SERVICE_WEATHER,  WEATHER},
        {SERVICE_CURRENCY, CURRENCY}
};

const map<Service, string> MAP_COMMAND_EXPLANATION = {
        {WEATHER,  "permite obtener la temperatura de una ciudad. Ejemplo: \"BuenosAires\""},
        {CURRENCY, "permite obtener el valor de la moneda de un pais con respecto al peso Argentino. Ejemplo: \"Peru\""}
};

const map<Operation, string> MAP_OPERATION = {
        {CREATE,  OPERATION_CREATE},
        {READ, OPERATION_READ},
        {UPDATE, OPERATION_UPDATE},
        {DELETE, OPERATION_DELETE},
        {END,      SERVICE_END}
};

const map<string, Operation> COMMAND_TO_OPERATION = {
        {OPERATION_CREATE,  CREATE},
        {OPERATION_READ,  READ},
        {OPERATION_UPDATE,  UPDATE},
        {OPERATION_DELETE,  DELETE},
        {SERVICE_END, END}
};

const map<Operation, string> MAP_OPERATION_EXPLANATION = {
        {CREATE, "permite agregar datos a la base. Se usa con crear [SERVICIO] [CLAVE] [VALOR]"},
        {READ,   "permite leer datos. Se usa con leer [SERVICIO] [CLAVE]"},
        {UPDATE, "permite actualizar datos. Se usa con editar [SERVICIO] [CLAVE] [NUEVO_VALOR]"},
        {DELETE, "permite eliminar datos. Se usa con eliminar [SERVICIO] [CLAVE]"},
        {END,    "despedirse."}
};

struct ClientRequest {
    ServiceOperation serviceOperation;
    char query[255];
    char value[255];
};

struct MicroserviceMessage {
    long type;
    long requesterIdentifier;
    Operation operation;
    char query[255];
    char value[255];
};

#endif //CONCU_NEWS_DEFINITION_H
