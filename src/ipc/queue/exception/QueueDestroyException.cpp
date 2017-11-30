#include <cstring>
#include "QueueDestroyException.h"

QueueDestroyException::QueueDestroyException(const string &description) {
    char *temp = new char[description.length() + 1];
    std::strcpy(temp, description.c_str());
    this->description = temp;
}

const char *QueueDestroyException::what() const throw() {
    return description;
}