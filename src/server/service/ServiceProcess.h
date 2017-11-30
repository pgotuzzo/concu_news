#ifndef CONCU_NEWS_SERVICEPROCESS_H
#define CONCU_NEWS_SERVICEPROCESS_H

#include <string>
#include <map>

using namespace std;

class ServiceProcess {
private:

    const char *const path_db;
    const char *const path_queue;
    const long msType;
    map<string, string> mInfo;

    void loadInfo();

    void saveInfo();

public:
    ServiceProcess(const char *const path_db, const char *const path_queue, const long msType);

    int live();
};


#endif //CONCU_NEWS_SERVICEPROCESS_H
