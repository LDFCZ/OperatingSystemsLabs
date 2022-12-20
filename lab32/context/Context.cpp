//
// Created by ldfcz on 12.12.22.
//

#include "Context.h"
#include <vector>

void Proxy::Context::addClient(int clientFD) {
    auto* threadContext = new ThreadContext( _cashContext, clientFD);
    _threadMap.insert({threadContext->getTid(), threadContext});
}

int Proxy::Context::joinDeadClients() {
    std::vector<pthread_t> keysToDelete;
    for (auto itr = _threadMap.begin(); itr != _threadMap.end(); ++itr) {
        if(!itr->second->isWorking()){
            int joinCode = pthread_join(itr->first, nullptr);
            if (joinCode != CORRECT_CODE) {
                LOG_ERROR("join error: " + std::string(strerror(joinCode)));
            }
            delete itr->second;
            keysToDelete.push_back(itr->first);
        }
    }
    for (auto key: keysToDelete) {
        _threadMap.erase(key);
    }
    return _threadMap.size();
}

Proxy::Context::Context() {
    _cashContext = new CashContext();
}
