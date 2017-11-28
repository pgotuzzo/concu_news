#ifndef BEACH_VOLEY_SOCKETCREATIONEXCEPTION_H
#define BEACH_VOLEY_SOCKETCREATIONEXCEPTION_H

#include <exception>
#include <cstring>
#include <string>

using namespace std;

class SocketCreationException : public exception {
public:
    explicit SocketCreationException(const string &description);

    const char *what() const throw() override;

private:
    const char *description;
};


#endif //BEACH_VOLEY_SOCKETCREATIONEXCEPTION_H
