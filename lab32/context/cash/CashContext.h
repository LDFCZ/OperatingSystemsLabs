//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB32_CASHCONTEXT_H
#define LAB32_CASHCONTEXT_H

#include <cstring>
#include <map>
#include <pthread.h>
#include <string>

namespace Proxy {

    class CashContext {
    public:
        CashContext();

        ~CashContext();

        std::string findInCash(const std::string& request);

        void addToCash(const std::string& request, const std::string& response);

    private:
        pthread_mutex_t _mutex;
        std::map<std::string, std::string> _requestResponse;
    };

} // Proxy

#endif //LAB32_CASHCONTEXT_H
