#ifndef LAB31PROXY_CLIENTINTERFACE_H
#define LAB31PROXY_CLIENTINTERFACE_H
#include "../buffer/Buffer.h"
#include "../binaryString/BinaryString.h"
#include <sys/poll.h>
#include <memory>

namespace ProxyServer {
    typedef enum {
        USER,
        HTTP_SERVER
    } TypeClient;

    class Client {
    public:
        virtual ~Client() = default;

        virtual int getFdClient() = 0;

        virtual void sendBuf(std::string* buf) = 0;

        virtual void readBuf(std::string *buf) = 0;

        virtual Buffer *getBuffer() = 0;

        virtual void setEvents(int event) = 0;

        virtual void setBuffer(Buffer *buffer) = 0;

        virtual TypeClient getTypeClient() = 0;

        virtual struct pollfd getPollFd() = 0;

        virtual void setPollElement(struct pollfd pollfd) = 0;

        virtual void setReventsZero() = 0;
    };
}
#endif //LAB31PROXY_CLIENTINTERFACE_H
