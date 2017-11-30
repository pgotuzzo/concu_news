#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include <cstring>
#include "../util/Logger.h"
#include "../cons/Path.h"
#include "../server/ServerProcess.h"

using namespace std;

int main(int argc, char **argv) {
    Logger::init(PATH_LOG_SERVER, true);
    Logger::i("======================================================================");
    Logger::i("                   Concu News - Server                                ");
    Logger::i("======================================================================");
    ServerProcess server;
    int res = server.live();
    if (res != 0) {
        stringstream msg;
        msg << "Se finalizo de forma abrupta/erronea. Error: " << res << strerror(res);
        Logger::e(msg.str());
    } else {
        Logger::i("Finalizo correctamente!");
        Logger::i("======================================================================");
    }

    exit(0);
}
