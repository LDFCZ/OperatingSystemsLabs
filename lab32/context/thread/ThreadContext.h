//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB32_THREADCONTEXT_H
#define LAB32_THREADCONTEXT_H

#include "../cash/CashContext.h"
#include "../../logger/Logger.h"
#include "../../Consts.h"
#include "../../parser/ParserImpl.h"
#include "../../exception/ConnectException.h"

#include <pthread.h>
#include <list>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

namespace Proxy {
    void* run(void*);

    class ThreadContext {
    public:
        ThreadContext(CashContext *cashContext , int socketFS);

        void startRoutine();

        pthread_t getTid() const;

        bool isWorking() const;

    private:
        CashContext* _cashContext;
        pthread_t _tid = 0;
        bool _isWorking;
        int _socketFD;

        int handleRequest(const std::string& request);

        int createHTTPSocket(const std::string& host, int port);

        int sendResponse(const std::string& response);

        int readResponse(int httpSocket, std::string *response);
    };

    inline void* run(void* args) {
        LOG_EVENT("new thread starts");
        auto* threadContext = (ThreadContext*)args;
        threadContext->startRoutine();
        pthread_exit(nullptr);
    }

} // Proxy

#endif //LAB32_THREADCONTEXT_H
