//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB32_CONSTS_H
#define LAB32_CONSTS_H

#include <string>

namespace Proxy {
    static const int CORRECT_CODE = 0;
    static const int ERROR_CODE = -1;

    static const int TIME_OUT_POLL = 10000;
    static const int PORT_SERVER_SOCKET = 8099;
    static const int MAX_COUNT_CONNECTIONS = 65000;
    static const long long int BUF_SIZE = 1024 * 1024;

    static int DEFAULT_PORT = 80;
    static int COUNT_CASH_ELEMENT = 100;
    static long long int SIZE_EACH_CASH_ELEMENT = 9223372036854775000;

    static long long int MAX_CASH_SIZE = 1024 * 1024 * 1024; // 1 Gb?

    static std::string REGEX_FOR_HOST_AND_PORT = "Host: ([\\w.\\d-]*)(:(\\d)+)*";
    static std::string REGEX_FOR_CONTENT_LENGTH = "Content-Length: ([\\d]*)";

} // Proxy

#endif //LAB32_CONSTS_H
