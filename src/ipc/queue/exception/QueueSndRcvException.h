#ifndef CONCU_NEWS_QUEUESNDRCVEXCEPTION_H
#define CONCU_NEWS_QUEUESNDRCVEXCEPTION_H

#include <exception>
#include <cstring>
#include <string>

using namespace std;

class QueueSndRcvException : public exception {
public:
    explicit QueueSndRcvException(const string &description);

    const char *what() const throw() override;

private:
    const char *description;
};


#endif //CONCU_NEWS_QUEUESNDRCVEXCEPTION_H
