//
// Created by ldfcz on 07.12.22.
//

#include "ServerSocketImpl.h"

namespace Proxy {
    int ServerSocketImpl::connectSocket() {
        int sockFd = 0;
        if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            LOG_ERROR_WITH_ERRNO("create socket");
            throw ConnectException("bind socket");
        }

        const int enable = 1;
        if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
            LOG_ERROR_WITH_ERRNO("setsockopt(SO_REUSEADDR) failed");
            throw ConnectException("setsockopt(SO_REUSEADDR) failed");
        }
        if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
            LOG_ERROR_WITH_ERRNO("setsockopt(SO_REUSEADDR) failed");
            throw ConnectException("setsockopt(SO_REUSEADDR) failed");
        }

        struct sockaddr_in sockAddr{};
        sockAddr.sin_port = htons(PORT_SERVER_SOCKET);
        sockAddr.sin_addr.s_addr = INADDR_ANY;
        sockAddr.sin_family = AF_INET;

        if (bind(sockFd, (struct sockaddr *) &sockAddr, sizeof(struct sockaddr_in)) < 0) {
            LOG_ERROR_WITH_ERRNO("bind socket");
            throw ConnectException("bind socket");
        }

        if (listen(sockFd, 3) < 0) {
            LOG_ERROR_WITH_ERRNO("listen socket");
            throw ConnectException("listen socket");
        }
        _serverSocketFD = sockFd;
        return 0;
    }

    int ServerSocketImpl::getFdSocket() {
        return _serverSocketFD;
    }

    void ServerSocketImpl::acceptNewClient(Context* context) {
        int clientSock = 0;
        struct sockaddr clientAddr;
        socklen_t len = 0;
        if ((clientSock = accept(_serverSocketFD, (struct sockaddr *) &clientAddr, &len)) < 0) {
            LOG_ERROR_WITH_ERRNO("accept new client");
            throw ConnectException("accept new client");
        }

        context->addClient(clientSock);
        LOG_EVENT("accept new client");
    }

    //Client *ServerSocketImpl::connectToClient(std::string url, int port) {
    //    return nullptr;
    //}

    void ServerSocketImpl::closeSocket() {
        close(_serverSocketFD);
    }
} // Proxy