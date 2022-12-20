//
// Created by ldfcz on 07.12.22.
//

#include "CashContext.h"
#include "../../Consts.h"
#include "../../logger/Logger.h"
#include "../../exception/CashContextException.h"

namespace Proxy {
    std::string CashContext::findInCash(const std::string& request) {
        std::string response = "";
        int lockCode = pthread_mutex_lock(&_mutex);
        if (lockCode != CORRECT_CODE) {
            LOG_ERROR("mutex lock error: " + std::string(strerror(lockCode)));
            return "";
        }

        if(_requestResponse.contains(request))
            response = _requestResponse[request];

        int unlockCode = pthread_mutex_unlock(&_mutex);
        if (unlockCode != CORRECT_CODE) {
            LOG_ERROR("mutex unlock error: " + std::string(strerror(lockCode)));
        }
        return response;
    }

    void CashContext::addToCash(const std::string& request, const std::string& response) {
        int lockCode = pthread_mutex_lock(&_mutex);
        if (lockCode != CORRECT_CODE) {
            LOG_ERROR("mutex lock error: " + std::string(strerror(lockCode)));
            return;
        }

        if(!_requestResponse.contains(request))
            _requestResponse.insert({request, response});

        int unlockCode = pthread_mutex_unlock(&_mutex);
        if (unlockCode != CORRECT_CODE) {
            LOG_ERROR("mutex unlock error: " + std::string(strerror(lockCode)));
        }
    }

    CashContext::CashContext() {
        pthread_mutexattr_t mutexAttr;
        int attrInitCode = pthread_mutexattr_init(&mutexAttr);
        if (attrInitCode != CORRECT_CODE) {
            LOG_ERROR("mutex attr init error: " + std::string(strerror(attrInitCode)));
            throw CashContextException("mutex attr init error: " + std::string(strerror(attrInitCode)));
        }

        int setTypeCode = pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_ERRORCHECK);
        if (setTypeCode != CORRECT_CODE) {
            LOG_ERROR("mutex attr settype error: " + std::string(strerror(setTypeCode)));
            pthread_mutexattr_destroy(&mutexAttr);
            throw CashContextException("mutex attr settype error: " + std::string(strerror(setTypeCode)));
        }

        int mutexInitCode = pthread_mutex_init(&_mutex, &mutexAttr);
        if (mutexInitCode != CORRECT_CODE) {
            LOG_ERROR("mutex init error: " + std::string(strerror(mutexInitCode)));
            pthread_mutexattr_destroy(&mutexAttr);
            throw CashContextException("mutex init error: " + std::string(strerror(mutexInitCode)));
        }
    }

    CashContext::~CashContext() {
        pthread_mutex_destroy(&_mutex);
    }
} // Proxy