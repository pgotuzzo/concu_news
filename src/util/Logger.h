#ifndef CONCU_NEWS_LOGGER_H
#define CONCU_NEWS_LOGGER_H

#include <iostream>

using namespace std;

class Logger {
private:
    const static char *sPath;

    static void log(const char *tag, string level, string message);

public:
    static void init(const char *path, bool reset);

    static void i(string message);

    static void i(const char *tag, string message);

    static void d(string message);

    static void d(const char *tag, string message);

    static void w(string message);

    static void w(const char *tag, string message);

    static void e(string message);

    static void e(const char *tag, string message);
};


#endif //CONCU_NEWS_LOGGER_H
