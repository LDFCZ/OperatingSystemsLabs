#include <iostream>
#include "server/ServerImpl.h"


int main() {
    LOG_EVENT("start program");
    try {
        auto *server = new Proxy::ServerImpl();
        server->startServer();
    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
        LOG_ERROR("close app by exception");
    }
    LOG_EVENT("close app");
    LOGGER_CLOSE;
    pthread_exit(nullptr);
}
