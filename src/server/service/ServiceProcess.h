#ifndef CONCU_NEWS_SERVICEPROCESS_H
#define CONCU_NEWS_SERVICEPROCESS_H

#include <string>
#include <map>

using namespace std;

class ServiceProcess {
private:
    const char *const path;
    map<string, string> mInfo;

    void loadInfo();

    void saveInfo();

public:
    ServiceProcess(const char *path);

    int live();
};


#endif //CONCU_NEWS_SERVICEPROCESS_H
