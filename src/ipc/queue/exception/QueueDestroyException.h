#ifndef CONCU_NEWS_QUEUEDESTROYEXCEPTION_H
#define CONCU_NEWS_QUEUEDESTROYEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class QueueDestroyException : public exception {
public:
    explicit QueueDestroyException(const string &description);

    const char *what() const throw() override;

private:
    const char *description;
};


#endif //CONCU_NEWS_QUEUEDESTROYEXCEPTION_H
