#ifndef CONCU_NEWS_DEFINITION_H
#define CONCU_NEWS_DEFINITION_H

#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include "Constants.h"

using namespace std;

enum Operation {
    WEATHER = 3,
    CURRENCY = 4,
    END = 5
};

const map<Operation, string> MAP_COMMAND = {
        {WEATHER,  OPERATION_WEATHER},
        {CURRENCY, OPERATION_CURRENCY},
        {END,      OPERATION_END}
};

const map<Operation, string> MAP_COMMAND_EXPLANATION = {
        {WEATHER,  "permite obtener la temperatura de una ciudad. Ejemplo: \"BuenosAires\""},
        {CURRENCY, "permite obtener el valor de la moneda de un pais con respecto al peso Argentino. Ejemplo: \"Brasil\""},
        {END,      "despedirse"}
};

struct ClientRequest {
    Operation operation;
    char query[255];
    char value[255];
};

struct MicroserviceRequest {
    long type;
    long requesterIdentifier;
    char query[255];
    char value[255];
};

#endif //CONCU_NEWS_DEFINITION_H
