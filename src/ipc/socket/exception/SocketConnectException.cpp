#include "SocketConnectException.h"

using namespace std;

SocketConnectException::SocketConnectException(const string &description) {
    char *temp = new char[description.length() + 1];
    std::strcpy(temp, description.c_str());
    this->description = temp;
}

const char *SocketConnectException::what() const throw() {
    return description;
}