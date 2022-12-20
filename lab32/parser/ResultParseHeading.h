#ifndef LAB31PROXY_RESULTPARSEHEADING_H
#define LAB31PROXY_RESULTPARSEHEADING_H

#include <string>
#include "ParserTypeRequest.h"

namespace Proxy {
    class ResultParseHeading {
    public:
        const std::string &getHostName() const {
            return _hostName;
        }

        void setHostName(const std::string &hostName) {
            ResultParseHeading::_hostName = hostName;
        }

        long long int getContentLength() const {
            return _contentLength;
        }

        void setContentLength(long long int contentLength) {
            ResultParseHeading::_contentLength = contentLength;
        }

        TypeRequestAndResponse getType() const {
            return _type;
        }

        void setType(TypeRequestAndResponse type) {
            ResultParseHeading::_type = type;
        }

        ~ResultParseHeading() = default;

        bool isResponseWithError() const {
            return _responseWithError;
        }

        void setResponseWithError(bool responseWithError) {
            ResultParseHeading::_responseWithError = responseWithError;
        }

        int getPort() const {
            return _port;
        }

        void setPort(int port) {
            ResultParseHeading::_port = port;
        }

        bool isHaveContentLength() const {
            return _haveContentLength;
        }

        void setHaveContentLength(bool haveContentLength) {
            ResultParseHeading::_haveContentLength = haveContentLength;
        }

    private:
        std::string _hostName;
        long long int _contentLength = 0;
        TypeRequestAndResponse _type;
        bool _responseWithError = false;
        int _port = 80;
        bool _haveContentLength = false;

    };

}
#endif //LAB31PROXY_RESULTPARSEHEADING_H
