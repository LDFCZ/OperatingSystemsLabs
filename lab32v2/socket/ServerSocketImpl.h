//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB31PROXY_SERVERSOCKETIMPL_H
#define LAB31PROXY_SERVERSOCKETIMPL_H

#include "ServerSocket.h"
#include "../logger/Logger.h"
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
#include <pthread.h>
#include "../Constants.h"
#include "../client/Client.h"
#include "../client/ClientImpl.h"
#include "../buffer/Buffer.h"
#include "../buffer/BufferImpl.h"

namespace ProxyServer {
    static pthread_mutex_t mutexForServer = PTHREAD_MUTEX_INITIALIZER;

    class ServerSocketImpl : public ServerSocket {
    public:
        ServerSocketImpl();

        int connectSocket() override;

        int getFdSocket() override;

        static Client *connectToClient(std::string url, int port);

        Client *acceptNewClient(Cash *cash) override;

        int acceptNewClientSock() override;

        void closeSocket() override;

        ~ServerSocketImpl();

    private:
        int serverSocket_ = 0;
    };
}


#endif //LAB31PROXY_SERVERSOCKETIMPL_H
