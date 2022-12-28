//
// Created by ldfcz on 07.12.22.
//

#include "CashImpl.h"
#include "CashElementImpl.h"
#define SUCCESS 0


ProxyServer::CashElement *ProxyServer::CashImpl::findResponseInCash(std::string heading) {
    std::hash<std::string> hasher;
    size_t hashHeading = hasher(heading);
    int lockCode = pthread_mutex_lock(&mutex);
    if (lockCode != SUCCESS) {
        LOG_ERROR("mutex lock error: " + std::string(strerror(lockCode)));
        return nullptr;
    }
    for (auto it = _listCash.begin(); it != _listCash.end(); it++) {
        if ((*it)->getHash() == hashHeading) {
            _listCash.splice(_listCash.end(), _listCash, it);
            LOG_EVENT("find in cash");
            CashElement* local = (*it);
            int unlockCode = pthread_mutex_unlock(&mutex);
            if (unlockCode != SUCCESS) {
                LOG_ERROR("mutex lock error: " + std::string(strerror(unlockCode)));
            }
            return local;
        }
    }
    int unlockCode =  pthread_mutex_unlock(&mutex);
    if (unlockCode != SUCCESS) {
        LOG_ERROR("mutex lock error: " + std::string(strerror(unlockCode)));
    }
    LOG_EVENT("not find in cash");
    return nullptr;
}

ProxyServer::CashElement *ProxyServer::CashImpl::addStringToCash(std::string request, long long int dataSize) {
    dataSize += request.length();
    int lockCode = pthread_mutex_lock(&mutex);
    if (lockCode != SUCCESS) {
        LOG_ERROR("mutex lock error: " + std::string(strerror(lockCode)));
        return nullptr;
    }
    if (dataSize > SIZE_EACH_CASH_ELEMENT) {
        int unlockCode = pthread_mutex_unlock(&mutex);
        if (unlockCode != SUCCESS) {
            LOG_ERROR("mutex unlock error: " + std::string(strerror(unlockCode)));
        }
        return nullptr;
    }
    if (byteInCash + dataSize >= MAX_CASH_SIZE) {
        for (auto it = _listCash.begin(); it != _listCash.end(); it++) {
            if ((*it)->getCountUsers() <= 0) {
                byteInCash -= (*it)->getLength();
                byteInCash -= (*it)->getHeading().length();
                it = _listCash.erase(it);
                if (byteInCash + dataSize < MAX_CASH_SIZE) {
                    break;
                }
            }
        }
        if (byteInCash + dataSize >= MAX_CASH_SIZE) {
            int unlockCode = pthread_mutex_unlock(&mutex);
            if (unlockCode != SUCCESS) {
                LOG_ERROR("mutex unlock error: " + std::string(strerror(unlockCode)));
            }
            return nullptr;
        }
    }
    LOG_EVENT("add to cash");
    byteInCash += dataSize;
    auto *cash = new CashElementImpl(request, dataSize);
    _listCash.push_back(cash);
    int unlockCode = pthread_mutex_unlock(&mutex);
    if (unlockCode != SUCCESS) {
        LOG_ERROR("mutex unlock error: " + std::string(strerror(unlockCode)));
    }
    return cash;
}

ProxyServer::CashImpl::~CashImpl() {
    for (auto &it: _listCash) {
        delete it;
    }
    int destroyCode = pthread_mutex_destroy(&mutex);
    if (destroyCode != SUCCESS) {
        LOG_ERROR("mutex destroy error: " + std::string(strerror(destroyCode)));
    }
}

bool ProxyServer::CashImpl::isElementInCash(std::string request) {
    std::hash<std::string> hasher;
    size_t hashHeading = hasher(request);
    int lockCode = pthread_mutex_lock(&mutex);
    if (lockCode != SUCCESS) {
        LOG_ERROR("mutex lock error: " + std::string(strerror(lockCode)));
        return false;
    }
    for (auto & it : _listCash) {
        if (it->getHash() == hashHeading) {
            int unlockCode = pthread_mutex_unlock(&mutex);
            if (unlockCode != SUCCESS) {
                LOG_ERROR("mutex unlock error: " + std::string(strerror(unlockCode)));
            }
            return true;
        }
    }
    int unlockCode = pthread_mutex_unlock(&mutex);
    if (unlockCode != SUCCESS) {
        LOG_ERROR("mutex unlock error: " + std::string(strerror(unlockCode)));
    }
    return false;
}

ProxyServer::CashImpl::CashImpl() {
    int initCode = pthread_mutex_init(&mutex, nullptr);
    if (initCode != SUCCESS) {
        LOG_ERROR("mutex init error: " + std::string(strerror(initCode)));
    }
}

