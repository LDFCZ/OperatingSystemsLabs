//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB32_SERVER_H
#define LAB32_SERVER_H

namespace Proxy {

    class Server {
        virtual void startServer() = 0;

        virtual void stopServer() = 0;
    };

} // Proxy

#endif //LAB32_SERVER_H
