//
// Created by ldfcz on 07.12.22.
//

#include "NewServerImpl.h"
#include <pthread.h>

using namespace ProxyServer;

void NewServerImpl::startServer() {
    std::list<ArgsForThread> listFd;
    while (true) {
        try {
            int sock = _serverSocket->acceptNewClientSock();
            listFd.push_front(ArgsForThread(sock, _cash));
            pthread_t pthread;
            int createCode = pthread_create(&pthread, nullptr, &NewServerImpl::startingMethodForThread,
                                   (void *) &(*listFd.begin()));
            if (createCode != SUCCESS) {
                LOG_ERROR("create error: " + std::string(strerror(createCode)));
                continue;
            }

            int detachCode = pthread_detach(pthread);
            if (detachCode != SUCCESS) {
                LOG_ERROR("detach error: " + std::string(strerror(detachCode)));
                continue;
            }
        } catch (std::exception *exception) {
            std::cerr << exception->what() << std::endl;
            LOG_ERROR("exception in connect");
        }
    }
}

void *NewServerImpl::startingMethodForThread(void *args) {
    auto *argsForThread = (ArgsForThread *) args;
    Client *client = new ClientImpl(argsForThread->getSock(), TypeClient::USER,
                                    new BufferImpl(argsForThread->getCash()));
    client->getBuffer()->setIsClientConnect(true);
    HandlerOneClientImpl handlerOneClient = HandlerOneClientImpl(client);
    handlerOneClient.startHandler();
    pthread_exit(nullptr);
}

NewServerImpl::NewServerImpl() {
    _serverSocket = new ServerSocketImpl();
    _serverSocket->connectSocket();
    _cash = new CashImpl();
}

NewServerImpl::~NewServerImpl() {
    _serverSocket->closeSocket();
    delete (ServerSocketImpl *) _serverSocket;
    delete _cash;
}

