#ifndef CONCU_NEWS_CONSTANTS_H
#define CONCU_NEWS_CONSTANTS_H

const static int SERVER_PORT = 16000;
const static char *const SERVER_IP = "127.0.0.1";

// Operations - Server-Client contract
const static char *const SERVICE_WEATHER = "tiempo";
const static char *const SERVICE_CURRENCY = "moneda";
const static char *const SERVICE_END = "fin";
const static char *const OPERATION_CREATE = "crear";
const static char *const OPERATION_READ = "leer";
const static char *const OPERATION_UPDATE = "editar";
const static char *const OPERATION_DELETE = "eliminar";

#endif //CONCU_NEWS_CONSTANTS_H
