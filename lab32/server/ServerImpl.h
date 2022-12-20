//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB32_SERVERIMPL_H
#define LAB32_SERVERIMPL_H

#include "Server.h"
#include "../logger/Logger.h"
#include <poll.h>
#include <iostream>
#include <map>
#include <list>
#include <memory>
#include <cstring>
#include "../Consts.h"
#include "../client/Client.h"
#include "../socket/ServerSocket.h"
#include "../socket/ServerSocketImpl.h"
#include "../context/Context.h"

#include <list>

namespace Proxy {

    class ServerImpl : public Server {
    public:
        ServerImpl();
        void startServer() override;
        void stopServer() override;
        ~ServerImpl();

    private:
        void setPolls();

        bool _isWorking = true;
        ServerSocket* _serverSocket;
        struct pollfd _socketPoll;
        struct pollfd _stdinPoll;
        //std::list<Client*> _clientList;
        Context* _context;
    };

} // Proxy

#endif //LAB32_SERVERIMPL_H
