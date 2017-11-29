#include "QueueCreationException.h"

using namespace std;

QueueCreationException::QueueCreationException(const string &description) {
    char *temp = new char[description.length() + 1];
    std::strcpy(temp, description.c_str());
    this->description = temp;
}

const char *QueueCreationException::what() const throw() {
    return description;
}