//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB32PROXY_ARGSFORTHREAD_H
#define LAB32PROXY_ARGSFORTHREAD_H

namespace ProxyServer {
    class ArgsForThread {
    public:
        int getSock() const {
            return _sock;
        }

        void setSock(int sock) {
            _sock = sock;
        }

        Cash *getCash() const {
            return _cash;
        }

        void setCash(Cash *cash) {
            _cash = cash;
        }

        int _sock;
        Cash *_cash;

        ArgsForThread(int sock, Cash *cash) {
            _cash = cash;
            _sock = sock;
        }
    };
}

#endif //LAB32PROXY_ARGSFORTHREAD_H
