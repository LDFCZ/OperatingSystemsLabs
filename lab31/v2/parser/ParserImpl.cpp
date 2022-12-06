#include <iostream>
#include <regex>
#include "ParserImpl.h"

using namespace ProxyServer;

ResultPars ParserImpl::findEndHeading(const std::string& buf, int *posEnd) {
    int resultFinding = buf.find("\r\n\r\n");
    if (resultFinding == std::string::npos) {
        *posEnd = -1;
        return ResultPars::NOTHING;
    } else {
        *posEnd = resultFinding + 4;
        return ResultPars::END_HEADING;
    }
}

TypeRequestAndResponse ParserImpl::parsingRequest(char *buf, char *host) {
    return NOT_GET_REQUEST;
}

ResultParseHeading *ParserImpl::parsingHeading(const std::string& heading) {
    auto *result = new ResultParseHeading;
    if (heading.size() > 3) {
        if (heading.substr(0, 3) == "GET") {
            result->setType(TypeRequestAndResponse::GET_REQUEST);
        } else {
            result->setType(TypeRequestAndResponse::NOT_GET_REQUEST);
        }
        findContentLength(result, heading);
        findHostAndPort(result, heading);
    } else {
        result->setType(TypeRequestAndResponse::INVALID);
        LOG_ERROR("incorrect heading");
        throw ParseException("incorrect heading");
        // TODO обработать ?
    }

    return result;
}

ResultParseHeading ParserImpl::parsingResponseHeading(const std::string& heading) {
    ResultParseHeading result; //HTTP/1.1 200 OK
    int statusResponse = heading.find("HTTP/1.1 200 OK");
    if (statusResponse == std::string::npos) {

        int test = heading.find("200 OK");
        if (test != std::string::npos) {
            result.setType(TypeRequestAndResponse::NORMAL_RESPONSE);
            result.setResponseWithError(false);
            findContentLength(&result, heading);
            return result;
        }

        result.setType(TypeRequestAndResponse::INVALID);
        result.setResponseWithError(true);
    } else {
        result.setType(TypeRequestAndResponse::NORMAL_RESPONSE);
        result.setResponseWithError(false);
    }
    findContentLength(&result, heading);
    return result;
}

ResultPars ParserImpl::findEndBody(const std::string& buffer, int *posEnd) {
    int resultFinding = buffer.find("0\r\n\r\n");
    if (resultFinding == std::string::npos) {
        *posEnd = -1;
        return ResultPars::NOTHING;
    } else {
        *posEnd = resultFinding + 5;
        return ResultPars::END_BODY;
    }
}

void ParserImpl::findHostAndPort(ResultParseHeading *result, const std::string& buf) {
    std::regex regex(REGEX_FOR_HOST_AND_PORT);
    std::smatch resRegex;

    if (regex_search(buf, resRegex, regex)) {
        if (resRegex.size() == 2) {
            result->setHostName(resRegex[1]);
        } else {
            result->setHostName(resRegex[1]);
            if (resRegex[2].str().size() > 1) {
                result->setPort(atoi(resRegex[2].str().substr(1).c_str()));
            } else {
                result->setPort(80);
            }
        }
        std::cout << "_port = " << result->getPort() << " host name = " << result->getHostName() << std::endl;
    } else {
        result->setType(TypeRequestAndResponse::INVALID);
        LOG_ERROR("incorrect heading");
        throw ParseException("incorrect heading");
    }
}

void ParserImpl::findContentLength(ResultParseHeading *result, const std::string& buf) {
    std::regex regex(REGEX_FOR_CONTENT_LENGTH);
    std::smatch resRegex;

    if (regex_search(buf, resRegex, regex)) {
        if (resRegex.size() == 2) {
            result->setContentLength(atoi(resRegex[1].str().c_str()));
            result->setHaveContentLength(true);
        }
    } else {
        result->setHaveContentLength(false);
    }
}
