#ifndef CONCU_NEWS_QUEUECREATIONEXCEPTION_H
#define CONCU_NEWS_QUEUECREATIONEXCEPTION_H

#include <exception>
#include <cstring>
#include <string>

using namespace std;

class QueueCreationException : public exception {
public:
    explicit QueueCreationException(const string &description);

    const char *what() const throw() override;

private:
    const char *description;
};


#endif //CONCU_NEWS_QUEUECREATIONEXCEPTION_H
