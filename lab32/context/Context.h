//
// Created by ldfcz on 12.12.22.
//

#ifndef LAB32_CONTEXT_H
#define LAB32_CONTEXT_H

#include "thread/ThreadContext.h"
#include "cash/CashContext.h"

#include <map>
#include <pthread.h>

namespace Proxy {

    class Context {
    public:
        Context();

        void addClient(int clientFD);

        int joinDeadClients();

    private:
        CashContext* _cashContext;
        std::map<pthread_t, ThreadContext*> _threadMap;
    };
}


#endif //LAB32_CONTEXT_H
