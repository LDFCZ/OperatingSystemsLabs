//
// Created by ldfcz on 07.12.22.
//

#include "ServerImpl.h"
#include "../cleaner/Cleaner.h"

namespace Proxy {
    ServerImpl::ServerImpl() {
        memset(&_socketPoll, 0, sizeof(struct pollfd));
        memset(&_stdinPoll, 0, sizeof(struct pollfd));
        _serverSocket = new ServerSocketImpl();
        _serverSocket->connectSocket();
        _context = new Context();
    }

    void ServerImpl::startServer() {
        LOG_EVENT("server starts");
        Cleaner::Cleaner cleaner(_context);
        setPolls();
        while (_isWorking) {
            int code = poll(&_socketPoll, 1, TIME_OUT_POLL);
            if (code == -1) {
                LOG_ERROR_WITH_ERRNO("poll error");
                perror("poll error");
                return;
            } else if (code == 0) {
                std::cout << "time out" << std::endl;
            } else {
                if (_socketPoll.revents & POLLIN) { // poll sock
                    _socketPoll.revents = 0;
                    try {
                        _serverSocket->acceptNewClient(_context);
                        LOG_EVENT("add new client");
                    } catch (std::exception *exception) {
                        std::cerr << exception->what() << std::endl;
                        LOG_ERROR("exception in connect");
                    }
                }
            }

            code = poll(&_stdinPoll, 1, 0);
            if (code == -1) {
                LOG_ERROR_WITH_ERRNO("poll error");
                perror("poll error");
                return;
            } else if (code != 0) {
                if (_stdinPoll.revents & POLLIN) {
                    _stdinPoll.revents = 0;
                    std::string in;
                    std::cin >> in;
                    if (in == "q") {
                        _isWorking = false;
                        LOG_EVENT("STOPPING SERVER!");
                    }
                }
            }
        }
        cleaner.stopRoutine();
    }

    ServerImpl::~ServerImpl() {
        _serverSocket->closeSocket();
        delete (ServerSocketImpl *) _serverSocket;
        delete _context;
    }

    void ServerImpl::setPolls() {
        memset(&_socketPoll, 0,sizeof(struct pollfd));
        _socketPoll.fd = _serverSocket->getFdSocket();
        _socketPoll.events = POLLIN;
        memset(&_stdinPoll, 0,sizeof(struct pollfd));
        _stdinPoll.fd = 0;
        _stdinPoll.events = POLLIN;
    }

    void ServerImpl::stopServer() {
        _isWorking = false;
    }
} // Proxy