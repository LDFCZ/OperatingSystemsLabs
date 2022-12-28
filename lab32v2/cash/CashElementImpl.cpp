//
// Created by ldfcz on 07.12.22.
//


#include "CashElementImpl.h"
#define SUCCESS 0

using namespace ProxyServer;

bool CashElementImpl::isDownloadEnd() {
    return _isDownloadEnd;
}

void CashElementImpl::setDownloadEnd(bool var) {
    _isDownloadEnd = var;
}

std::shared_ptr<std::string> CashElementImpl::getCash() {
    return _cash;
}

CashElementImpl::CashElementImpl(const std::string& heading, long long int dataSize) {
    int initCode = pthread_rwlock_init(&_mutexForData, nullptr);
    if (initCode != SUCCESS) {
        LOG_ERROR("rwlock init error: " + std::string(strerror(initCode)));
    }
    initCode = pthread_mutex_init(&_mutexForSubscribers, nullptr);
    if (initCode != SUCCESS) {
        LOG_ERROR("mutex init error: " + std::string(strerror(initCode)));
    }
    _requestHeading = heading;
    std::hash<std::string> hasher;
    _cash->resize(dataSize - heading.size());
    _cash->clear();
    _hashRequestHeading = hasher(heading);
}

long long int CashElementImpl::getHash() {
    return _hashRequestHeading;
}

CashElementImpl::~CashElementImpl() {
    LOG_EVENT("delete cash ");
    int destroyCode = pthread_rwlock_destroy(&_mutexForData);
    if (destroyCode != SUCCESS) {
        LOG_ERROR("rwlock destroy error: " + std::string(strerror(destroyCode)));
    }
    destroyCode = pthread_mutex_destroy(&_mutexForSubscribers);
    if (destroyCode != SUCCESS) {
        LOG_ERROR("mutex destroy error: " + std::string(strerror(destroyCode)));
    }
}

int CashElementImpl::getCountUsers() {
    return _countConnectedUsers;
}

void CashElementImpl::addCountUsers() {
    int lockCode = pthread_mutex_lock(&_mutexForSubscribers);
    if (lockCode != SUCCESS) {
        LOG_ERROR("mutex lock error: " + std::string(strerror(lockCode)));
        return;
    }
    _countConnectedUsers++;
    int unlockCode = pthread_mutex_unlock(&_mutexForSubscribers);
    if (unlockCode != SUCCESS) {
        LOG_ERROR("mutex unlock error: " + std::string(strerror(unlockCode)));
    }
}

void CashElementImpl::minusCountUsers() {
    int lockCode = pthread_mutex_lock(&_mutexForSubscribers);
    if (lockCode != SUCCESS) {
        LOG_ERROR("mutex lock error: " + std::string(strerror(lockCode)));
        return;
    }
    _countConnectedUsers--;
    int unlockCode = pthread_mutex_unlock(&_mutexForSubscribers);
    if (unlockCode != SUCCESS) {
        LOG_ERROR("mutex unlock error: " + std::string(strerror(unlockCode)));
    }
}

bool CashElementImpl::isIsServerConnected() {
    return _isServerConnected;
}

void CashElementImpl::setIsServerConnect(bool isServerConnected) {
    _isServerConnected = isServerConnected;

    if (!isServerConnected) {
        signalUsers();
    }

}

const std::string &CashElementImpl::getHeading() {
    return _requestHeading;
}

long long int CashElementImpl::getLength() {
    return _cash->size();
}

void CashElementImpl::memCopyFromCash(std::string *target, long long int offset,
                                      long long int sizeCopy) {
    pthread_rwlock_rdlock(&_mutexForData);
    memcpy((void *) (target)->c_str(), _cash->c_str() +
                                       offset, sizeCopy);
    int unlockCode = pthread_rwlock_unlock(&_mutexForData);
    if (unlockCode != SUCCESS) {
        LOG_ERROR("mutex unlock error: " + std::string(strerror(unlockCode)));
    }
}

void CashElementImpl::appendStringToCash(std::string *binaryString) {
    int lockCode = pthread_rwlock_wrlock(&_mutexForData);
    if (lockCode != SUCCESS) {
        LOG_ERROR("mutex lock error: " + std::string(strerror(lockCode)));
        return;
    }
    _cash->append(*binaryString);
    int unlockCode = pthread_rwlock_unlock(&_mutexForData);
    if (unlockCode != SUCCESS) {
        LOG_ERROR("mutex unlock error: " + std::string(strerror(unlockCode)));
    }
    signalUsers();
}

void CashElementImpl::addCondVar(pthread_cond_t *condVar) {
    int lockCode = pthread_mutex_lock(&_mutexForSubscribers);
    if (lockCode != SUCCESS) {
        LOG_ERROR("mutex lock error: " + std::string(strerror(lockCode)));
        return;
    }
    _listSubscribers.push_front(condVar);
    int unlockCode = pthread_mutex_unlock(&_mutexForSubscribers);
    if (unlockCode != SUCCESS) {
        LOG_ERROR("mutex unlock error: " + std::string(strerror(unlockCode)));
    }
}

void CashElementImpl::dellCondVar(pthread_cond_t *condVar) {
    int lockCode = pthread_mutex_lock(&_mutexForSubscribers);
    if (lockCode != SUCCESS) {
        LOG_ERROR("mutex lock error: " + std::string(strerror(lockCode)));
        return;
    }
    for (auto it = _listSubscribers.begin(); it != _listSubscribers.end(); it++) {
        if ((*it) == condVar) {
            _listSubscribers.erase(it);
            break;
        }
    }
    int unlockCode = pthread_mutex_unlock(&_mutexForSubscribers);
    if (unlockCode != SUCCESS) {
        LOG_ERROR("mutex unlock error: " + std::string(strerror(unlockCode)));
    }
}

void CashElementImpl::signalUsers() {
    int lockCode = pthread_mutex_lock(&_mutexForSubscribers);
    if (lockCode != SUCCESS) {
        LOG_ERROR("mutex lock error: " + std::string(strerror(lockCode)));
        return;
    }
    for (auto &item: _listSubscribers) {
        pthread_cond_signal(item);
    }
    int unlockCode = pthread_mutex_unlock(&_mutexForSubscribers);
    if (unlockCode != SUCCESS) {
        LOG_ERROR("mutex unlock error: " + std::string(strerror(unlockCode)));
    }
}
