#include "Logger.h"
#include "../cons/Path.h"
#include <fstream>

const char *Logger::sPath = PATH_LOG_DEFAULT;

void Logger::init(const char *path, bool reset) {
    if (reset) {
        remove(path);
    }
    sPath = path;
};

void Logger::log(const char *const tag, string level, string message) {
    std::ofstream salida(sPath, std::ios::out | std::ios::app);
    salida << level << "|\t" << tag << ": " << message << std::endl;
    salida.close();
}

void Logger::i(string message) {
    i("", message);
}

void Logger::i(const char *const tag, string message) {
    log(tag, "INFO", message);
}

void Logger::d(string message) {
    d("", message);
}

void Logger::d(const char *const tag, string message) {
    log(tag, "DEBUG", message);
}

void Logger::w(string message) {
    w("", message);
}

void Logger::w(const char *const tag, string message) {
    log(tag, "WARN", message);
}

void Logger::e(string message) {
    e("", message);
}

void Logger::e(const char *const tag, string message) {
    log(tag, "ERROR", message);
}

