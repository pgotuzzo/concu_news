#include <fcntl.h>
#include <fstream>
#include <zconf.h>
#include <cstring>
#include "ServiceProcess.h"
#include "../../util/FileUtils.h"

ServiceProcess::ServiceProcess(const char *const path) : path(path) {}

int ServiceProcess::live() {
    int pid = fork();
    if (pid != 0) {
        return pid;
    }
    loadInfo();

// FIXME - Eliminar...solo de prueba
//    for (pair<string, string> entry : mInfo) {
//        cout << entry.first << " " << entry.second << endl;
//    }

    saveInfo();
    exit(0);
}

void ServiceProcess::loadInfo() {
    createFileIfNotExists(path);
    ifstream file(path);
    string line;
    char key[255];
    char value[255];
    while (std::getline(file, line)) {
        memset(key, ' ', sizeof(char) * 255);
        memset(value, ' ', sizeof(char) * 255);
        sscanf(line.c_str(), "%s %s", key, value);
        mInfo[string(key)] = string(value);
    }
    file.close();
}

void ServiceProcess::saveInfo() {
    createFileIfNotExists(path);
    ofstream file(path);
    string line;
    for (pair<string, string> entry : mInfo) {
        file << entry.first << " " << entry.second << endl;
    }
    file.close();
}
