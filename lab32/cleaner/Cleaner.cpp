//
// Created by ldfcz on 07.12.22.
//

#include "Cleaner.h"

Cleaner::Cleaner::Cleaner(Proxy::Context *context) : _context(context), _isWorking(true) {
    int createCode = pthread_create(&_tid, nullptr, run, (void*)this);
    if (createCode != Proxy::CORRECT_CODE) {
        LOG_ERROR("create error: " + std::string(strerror(createCode)));
        _isWorking = false;
    }
}

void Cleaner::Cleaner::startRoutine() {
    int aliveTreadCount = 0;
    while (_isWorking || aliveTreadCount != 0){
        //LOG_EVENT("JOIN DEAD THREADS!");
        aliveTreadCount = _context->joinDeadClients();
        sleep(1);
    }
}

void Cleaner::Cleaner::stopRoutine() {
    _isWorking = false;
    int joinCode = pthread_join(_tid, nullptr);
    if (joinCode != Proxy::CORRECT_CODE) {
        LOG_ERROR("join error: " + std::string(strerror(joinCode)));
    }
}
