//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB31PROXY_NEWSERVERIMPL_H
#define LAB31PROXY_NEWSERVERIMPL_H


#include "Server.h"
#include "errno.h"
#include "../logger/Logger.h"
#include <iostream>
#include <map>
#include <list>
#include <memory>
#include <pthread.h>
#include "../Constants.h"
#include "../cash/CashImpl.h"
#include "../client/Client.h"
#include "../socket/ServerSocket.h"
#include "../socket/ServerSocketImpl.h"
#include "HandlerOneClient.h"
#include "HandlerOneClientImpl.h"
#include "ArgsForThread.h"

#define SUCCESS 0

namespace ProxyServer {
    class NewServerImpl : public Server {
    public:
        NewServerImpl();
        void startServer() override;
        ~NewServerImpl();
        static void *startingMethodForThread(void *args);
    private:

        ServerSocket* _serverSocket;
        Cash *_cash;
    };
}

#endif //LAB31PROXY_NEWSERVERIMPL_H
