#ifndef CONCU_NEWS_WORKERPROCESS_H
#define CONCU_NEWS_WORKERPROCESS_H


class WorkerProcess {
private:
    int fdClient;
public:
    WorkerProcess(int fdClient);

    int live();
};


#endif //CONCU_NEWS_WORKERPROCESS_H
