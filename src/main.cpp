#include <iostream>
#include <sstream>
#include "util/Logger.h"
#include "cons/Path.h"
#include "client/ClientProcess.h"
#include "server/ServerProcess.h"

using namespace std;

void showHelp() {
    FILE *pHelpFile;
    const int countMax = 100;
    char buffer[countMax];

    pHelpFile = fopen("help", "r");
    if (pHelpFile == nullptr) {
        perror("Error opening help file");
    } else {
        while (!feof(pHelpFile)) {
            char *res = fgets(buffer, countMax, pHelpFile);
            if (res != nullptr) {
                fputs(buffer, stdout);
            }
        }
        fclose(pHelpFile);
    }
    cout << endl;
}

void liveClient(string &userName, bool isAdmin) {
    string logPath(PATH_LOG_DIR + userName);
    Logger::init(logPath.c_str(), true);
    Logger::i("======================================================================");
    Logger::i("                 Concu News - Client " + userName);
    Logger::i("======================================================================");
    ClientProcess client(userName.c_str(), isAdmin);
    client.live();
    int res;
    wait(&res);
    if (res != 0) {
        stringstream msg;
        msg << "Se finalizo de forma abrupta/erronea. Error: " << res << strerror(res);
        Logger::e(msg.str());
    } else {
        Logger::i("Finalizo correctamente!");
        Logger::i("======================================================================");
    }
}

void liveServer() {
    Logger::init(PATH_LOG_SERVER, true);
    Logger::i("======================================================================");
    Logger::i("                   Concu News - Server                                ");
    Logger::i("======================================================================");
    ServerProcess server;
    server.live();
    int res;
    wait(&res);
    if (res != 0) {
        stringstream msg;
        msg << "Se finalizo de forma abrupta/erronea. Error: " << res << strerror(res);
        Logger::e(msg.str());
    } else {
        Logger::i("Finalizo correctamente!");
        Logger::i("======================================================================");
    }
}

int main(int argc, char **argv) {
    bool successfulInit = true;
    switch (argc) {
        case 1: {
            liveServer();
            break;
        }
        case 2: {
            string param = string(argv[1]);
            if (param == "-h") {
                showHelp();
            } else {
                liveClient(param, false);
            }
            break;
        }
        case 3: {
            string firstParam = string(argv[1]);
            string secondParam = string(argv[2]);
            if (firstParam == "-admin") {
                liveClient(secondParam, true);
            } else if (secondParam == "-admin") {
                liveClient(firstParam, true);
            } else {
                successfulInit = false;
            }
            break;
        }
        default: {
            successfulInit = false;
        }
    }
    if (!successfulInit) {
        cout << "Para saber como ejecutar Concu News consultar el manual de ayuda" << endl
             << "\t concu_news -h" << endl;
    }
    exit(0);
}
