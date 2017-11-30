#ifndef CONCU_NEWS_WORKERPROCESS_H
#define CONCU_NEWS_WORKERPROCESS_H


#include "../../ipc/handler/SIGTERM_SIGINT_QUIT_Handler.h"

class WorkerProcess {
private:
    int fdClient;
    SIGTERM_SIGINT_QUIT_Handler *termination_handler;
public:
    WorkerProcess(int fdClient, SIGTERM_SIGINT_QUIT_Handler *termination_handler);

    int live();
};


#endif //CONCU_NEWS_WORKERPROCESS_H
