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
#include "../Constants.h"
#include "../client/Client.h"
#include "../client/ClientImpl.h"
#include "../buffer/Buffer.h"
#include "../buffer/BufferImpl.h"

namespace ProxyServer {
    class ServerSocketImpl : public ServerSocket{
    public:
        int connectSocket() override;

        Client *connectToClient(std::string url, int port) override;

        int getFdSocket() override;

        Client* acceptNewClient(Cash* cash) override;

        void closeSocket() override;

        ~ServerSocketImpl();

    private:
        int serverSocket_ = 0;
    };
}


#endif //LAB31PROXY_SERVERSOCKETIMPL_H
