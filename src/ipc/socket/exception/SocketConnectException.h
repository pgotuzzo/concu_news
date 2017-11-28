#ifndef CONCU_NEWS_SOCKETCONNECTEXCEPTION_H
#define CONCU_NEWS_SOCKETCONNECTEXCEPTION_H

#include <exception>
#include <cstring>
#include <string>

using namespace std;

class SocketConnectException : public exception {
public:
    explicit SocketConnectException(const string &description);

    const char *what() const throw() override;

private:
    const char *description;
};


#endif //CONCU_NEWS_SOCKETCONNECTEXCEPTION_H
