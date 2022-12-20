//
// Created by ldfcz on 07.12.22.
//

#include "NewServerImpl.h"
#include <pthread.h>

// TODO not errno

using namespace ProxyServer;

void NewServerImpl::startServer() {
    std::list<ArgsForThread> listFd;
    while (true) {
        try {
            int sock = _serverSocket->acceptNewClientSock();
            listFd.push_front(ArgsForThread(sock, _cash));
            pthread_t pthread;
            errno = pthread_create(&pthread, NULL, &NewServerImpl::startingMethodForThread,
                                   (void *) &(*listFd.begin()));
            if (errno != SUCCESS) {
                perror("pthread_create error");
                continue;
                errno = SUCCESS;
            }
//            pthread_join(pthread, NULL);
            errno = pthread_detach(pthread);
            if (errno != SUCCESS) {
                perror("pthread_create error");
                errno = SUCCESS;
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

