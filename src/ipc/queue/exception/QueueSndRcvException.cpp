#include "QueueSndRcvException.h"

QueueSndRcvException::QueueSndRcvException(const string &description) {
    char *temp = new char[description.length() + 1];
    std::strcpy(temp, description.c_str());
    this->description = temp;
}

const char *QueueSndRcvException::what() const throw() {
    return description;
}