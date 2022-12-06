#include "CashImpl.h"
#include "CashElementImpl.h"
#include "../logger/Logger.h"

ProxyServer::CashElement *ProxyServer::CashImpl::findResponseInCash(std::string heading) {
    std::hash<std::string> context;
    size_t hashHeading = context(heading);
    for (auto it = _listCash.begin(); it != _listCash.end(); it++) {
        if ((*it)->getHash() == hashHeading) {
            _listCash.splice(_listCash.end(), _listCash, it);
            LOG_EVENT("find in cash");
            return (*it);
        }
    }
    LOG_EVENT("not find in cash");
    return nullptr;
}

ProxyServer::CashElement *ProxyServer::CashImpl::addStringToCash(const std::string& request, unsigned long long int dataSize) {
    dataSize += request.length();
    if (_byteInCash + dataSize >= MAX_CASH_SIZE) {
        if (dataSize > SIZE_EACH_CASH_ELEMENT) {
            return nullptr;
        }
        for (auto it = _listCash.begin(); it != _listCash.end(); it++) {
            if ((*it)->getCountUsers() <= 0) {
                _byteInCash -= (*it)->getCash()->length();
                _byteInCash -= (*it)->getHead().length();
                it = _listCash.erase(it);
                if (_byteInCash + dataSize < MAX_CASH_SIZE) {
                    break;
                }
            }
        }
        if (_byteInCash + dataSize >= MAX_CASH_SIZE) {
            return nullptr;
        }
    }

    LOG_EVENT("add to cash");
    _byteInCash += dataSize;
    auto *cash = new CashElementImpl(request);
    _listCash.push_back(cash);
    return cash;
}

ProxyServer::CashImpl::~CashImpl() {
    for (auto &it: _listCash) {
        delete it;
    }
}

bool ProxyServer::CashImpl::isElementInCash(std::string request) {
    std::hash<std::string> context;
    size_t hashHeading = context(request);
    for (auto & it : _listCash) {
        if (it->getHash() == hashHeading) {
            return true;
        }
    }
    return false;
}

