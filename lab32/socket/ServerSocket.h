//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB32_SERVERSOCKET_H
#define LAB32_SERVERSOCKET_H

#include "../client/Client.h"
#include "../context/Context.h"

#include <string>

namespace Proxy {

    class ServerSocket {
    public:
        virtual int getFdSocket() = 0;

        virtual void acceptNewClient(Context* context) = 0;

        //virtual Client *connectToClient(std::string url, int port) = 0;

        virtual void closeSocket() = 0;

        virtual int connectSocket() = 0;
    };
}


#endif //LAB32_SERVERSOCKET_H
