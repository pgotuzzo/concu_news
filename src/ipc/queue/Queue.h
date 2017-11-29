#ifndef CONCU_NEWS_QUEUE_H
#define CONCU_NEWS_QUEUE_H

#include <sys/msg.h>
#include <sstream>
#include "exception/QueueCreationException.h"
#include "exception/QueueSndRcvException.h"
#include "exception/QueueDestroyException.h"
#include "../../util/FileUtils.h"

using namespace std;

template<class T>
class Queue {
private:
    int id;
public:
    Queue(const char *path);

    void send(T element) const;

    void receive(T *ptrElement, int type) const;

    void destroy();
};

template<class T>
Queue<T>::Queue(const char *const path) {
    createFileIfNotExists(path);
    key_t key = ftok(path, 'A');
    if (key == -1) {
        stringstream errorMsg;
        errorMsg << "Fallo la creacion de la key (ftok) necesaria para crear la Queue. Error " << errno << ": "
                 << strerror(errno);
        throw QueueCreationException(errorMsg.str());
    }
    id = msgget(key, IPC_CREAT | 0664);
    if (id == -1) {
        stringstream errorMsg;
        errorMsg << "Fallo la creacion de la Queue (msgget). Error " << errno << ": " << strerror(errno);
        throw QueueCreationException(errorMsg.str());
    }
}

template<class T>
void Queue<T>::send(T element) const {
    int res = msgsnd(id, &element, sizeof(T) - sizeof(long), 0);
    if (res == -1) {
        stringstream errorMsg;
        errorMsg << "Fallo el envio del mensage (msgsnd). Error " << errno << ": " << strerror(errno);
        throw QueueSndRcvException(errorMsg.str());
    }
}

template<class T>
void Queue<T>::receive(T *ptrElement, int type) const {
    ssize_t res = msgrcv(id, ptrElement, sizeof(T) - sizeof(long), type, 0);
    if (res == -1) {
        stringstream errorMsg;
        errorMsg << "Fallo la recepcion del mensage (msgrcv). Error " << errno << ": " << strerror(errno);
        throw QueueSndRcvException(errorMsg.str());
    }
}

template<class T>
void Queue<T>::destroy() {
    int res = msgctl(id, IPC_RMID, nullptr);
    if (res == -1) {
        stringstream errorMsg;
        errorMsg << "Fallo la destruccion de la Queue. Error " << errno << ": " << strerror(errno);
        throw QueueDestroyException(errorMsg.str());
    }
}

#endif //CONCU_NEWS_QUEUE_H
