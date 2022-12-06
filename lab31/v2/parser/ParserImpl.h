#ifndef LAB31PROXY_PARSERIMPL_H
#define LAB31PROXY_PARSERIMPL_H

#include "ParserTypeRequest.h"
#include "ResultParseHeading.h"
#include "ParseException.h"
#include "../logger/Logger.h"
#include "../Constants.h"

namespace ProxyServer {
    class ParserImpl {
    public:
        static TypeRequestAndResponse parsingRequest(char *buf, char* host);
        static ResultPars findEndHeading(const std::string& buf, int* posEnd);
        static ResultPars findEndBody(const std::string& buffer, int* posEnd);
        static ResultParseHeading* parsingHeading(const std::string& heading);
        static ResultParseHeading parsingResponseHeading(const std::string& heading);

    private:
        static void findHostAndPort(ResultParseHeading* result, const std::string& buf);
        static void findContentLength(ResultParseHeading* result, const std::string& buf);
    };
}


#endif //LAB31PROXY_PARSERIMPL_H
