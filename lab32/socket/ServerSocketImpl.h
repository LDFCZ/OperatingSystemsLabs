//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB32_SERVERSOCKETIMPL_H
#define LAB32_SERVERSOCKETIMPL_H

#include "ServerSocket.h"
#include "../logger/Logger.h"
#include "../Consts.h"
#include "../exception/ConnectException.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>


namespace Proxy {

    class ServerSocketImpl : public ServerSocket {
    public:
        int connectSocket() override;

        int getFdSocket() override;

        void acceptNewClient(Context* context) override;

        //Client *connectToClient(std::string url, int port) override;

        void closeSocket() override;
    private:
        int _serverSocketFD = 0;
    };

} // Proxy

#endif //LAB32_SERVERSOCKETIMPL_H
