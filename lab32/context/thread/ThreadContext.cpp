//
// Created by ldfcz on 07.12.22.
//

#include "ThreadContext.h"

namespace Proxy {
    void ThreadContext::startRoutine() {
        while (_isWorking) {
            std::string buf;
            char readBuf[BUF_SIZE] = {0};
            int byte = read(_socketFD, readBuf, BUF_SIZE - 1);
            if (byte <= 0) {
                if (byte < 0)
                    LOG_ERROR_WITH_ERRNO("read error");
                _isWorking = false;
                close(_socketFD);
                LOG_EVENT("client disconnect " +  std::to_string(_tid));
                return;
            }
            buf.resize(byte, ' ');
            std::memcpy((void *) buf.c_str(), readBuf, byte);
            if (handleRequest(buf) != CORRECT_CODE) {
                LOG_ERROR("handle request error");
                _isWorking = false;
                close(_socketFD);
                LOG_EVENT("client disconnect");
                return;
            }
        }
    }

    ThreadContext::ThreadContext(CashContext *cashContext, int socketFD) :
        _cashContext(cashContext),
        _socketFD(socketFD),
        _isWorking(true){
        int createCode = pthread_create(&_tid, nullptr, run, (void*)this);
        if (createCode != CORRECT_CODE) {
            LOG_ERROR("create error: " + std::string(strerror(createCode)));
            _isWorking = false;
            close(_socketFD);
        }
    }

    pthread_t ThreadContext::getTid() const {
        return _tid;
    }

    bool ThreadContext::isWorking() const {
        return _isWorking;
    }

    int ThreadContext::handleRequest(const std::string& request) {
        LOG_EVENT("try found in cash " +  std::to_string(_tid));
        std::string findResult = _cashContext->findInCash(request);
        if (!findResult.empty()) {
            LOG_EVENT("found in cash! " +  std::to_string(_tid));
            int sendCode = sendResponse(findResult);
            if (sendCode != CORRECT_CODE){
                return ERROR_CODE;
            }
            return CORRECT_CODE;
        }
        LOG_EVENT("parse request " +  std::to_string(_tid));
        ResultParseHeading *res = ParserImpl::parsingHeading(request);
        int httpSocket = createHTTPSocket(res->getHostName(), res->getPort());
        if (httpSocket == ERROR_CODE) {
            return ERROR_CODE;
        }
        LOG_EVENT("write request " +  std::to_string(_tid));
        int byte = write(httpSocket, request.c_str(), request.length());
        if (byte < 0) {
            close(httpSocket);
            LOG_ERROR_WITH_ERRNO("write error");
            return ERROR_CODE;
        }

        std::string response;
        LOG_EVENT("read response " +  std::to_string(_tid));
        int readCode = readResponse(httpSocket, &response);
        close(httpSocket);
        if (readCode != CORRECT_CODE) {
            LOG_ERROR_WITH_ERRNO("read error");
            return ERROR_CODE;
        }
        LOG_EVENT("add to cash");
        _cashContext->addToCash(request, response);
        LOG_EVENT("send response");
        byte = write(_socketFD, response.c_str(), response.length());
        if (byte < 0) {
            LOG_ERROR_WITH_ERRNO("write error");
            return ERROR_CODE;
        }
        return CORRECT_CODE;
    }

    int ThreadContext::createHTTPSocket(const std::string& host, int port) {
        struct hostent *hostent = gethostbyname(host.data());
        if (hostent == nullptr) {
            LOG_ERROR("gethostbyname");
            herror("gethostbyname");
            return ERROR_CODE;
        }

        struct sockaddr_in sockAddr;
        bcopy(hostent->h_addr, &sockAddr.sin_addr, hostent->h_length);
        sockAddr.sin_port = htons(port);
        sockAddr.sin_family = AF_INET;

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == ERROR_CODE) {
            LOG_ERROR_WITH_ERRNO("setsockopt");
            return ERROR_CODE;
        }
        if (connect(sock, (struct sockaddr *) &sockAddr, sizeof(struct sockaddr_in)) == ERROR_CODE) {
            LOG_ERROR_WITH_ERRNO("connect: ");
            close(sock);
            return ERROR_CODE;
        }

        LOG_EVENT("http server connect");
        return  sock;
    }

    int ThreadContext::sendResponse(const std::string &response) {
        int byte = write(_socketFD, response.c_str(), response.length());
        if (byte < 0) {
            LOG_ERROR_WITH_ERRNO("write error");
            return ERROR_CODE;
        }
        LOG_EVENT("send response");
        return  CORRECT_CODE;
    }

    int ThreadContext::readResponse(int httpSocket, std::string *response) {
        int byte;
        char readBuf[BUF_SIZE] = {0};
        while ((byte = read(httpSocket, readBuf, BUF_SIZE - 1) ) > 0) {
            LOG_EVENT("read bytes: " + std::to_string(byte));
            if (byte < 0) {
                close(httpSocket);
                LOG_ERROR_WITH_ERRNO("read error");
                return ERROR_CODE;
            }
            std::string tmp;
            tmp.resize(byte, ' ');
            std::memcpy((void *) tmp.c_str(), readBuf, byte);
            *response += tmp;
            if (response->ends_with("\r\n\r\n")) return CORRECT_CODE;
        }
        return CORRECT_CODE;
    }
} // Proxy