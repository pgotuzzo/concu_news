#ifndef SIGTERM_SIGINT_QUIT_HANDLER_H
#define SIGTERM_SIGINT_QUIT_HANDLER_H

#include <csignal>
#include <cassert>

#include "EventHandler.h"

class SIGTERM_SIGINT_QUIT_Handler : public EventHandler {
private:
    sig_atomic_t gracefulQuit;

public:

    SIGTERM_SIGINT_QUIT_Handler() : gracefulQuit(0) {
    }

    virtual int handleSignal(int signum) {
        assert (signum == SIGINT || signum == SIGTERM);
        bloquear_sigs();
        this->gracefulQuit = 1;
        return 0;
    }

    sig_atomic_t getGracefulQuit() const {
        return this->gracefulQuit;
    }

    void bloquear_sigs() {
        sigset_t sa{};
        sigemptyset(&sa);
        sigaddset(&sa, SIGINT);
        sigaddset(&sa, SIGTERM);
        sigprocmask(SIG_BLOCK, &sa, NULL);
    }
};

#endif /* SIGTERM_SIGINT_QUIT_HANDLER_H */
