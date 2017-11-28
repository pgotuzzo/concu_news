#include <cstring>
#include "SocketCreationException.h"

using namespace std;

SocketCreationException::SocketCreationException(const string &description) {
    char *temp = new char[description.length() + 1];
    std::strcpy(temp, description.c_str());
    this->description = temp;
}

const char *SocketCreationException::what() const throw() {
    return description;
}