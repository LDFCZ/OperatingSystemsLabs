//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB32_CLEANER_H
#define LAB32_CLEANER_H

#include "../logger/Logger.h"
#include "../context/Context.h"
#include "../Consts.h"

#include <pthread.h>

namespace Cleaner {

    class Cleaner {
    public:
        Cleaner(Proxy::Context* context);

        void startRoutine();

        void stopRoutine();

    private:
        Proxy::Context* _context;
        pthread_t _tid;
        bool _isWorking;
    };

    inline void* run(void* args) {
        LOG_EVENT("new thread starts");
        auto* cleaner = (Cleaner*)args;
        cleaner->startRoutine();
        pthread_exit(nullptr);
    }
} // Proxy

#endif //LAB32_CLEANER_H
