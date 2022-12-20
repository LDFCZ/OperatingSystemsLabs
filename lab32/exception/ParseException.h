#ifndef LAB31PROXY_PARSEEXCEPTION_H
#define LAB31PROXY_PARSEEXCEPTION_H

#include <exception>
#include <string>

namespace Proxy {
    class ParseException : public std::exception {
    public:
        explicit ParseException(std::string msg) : msg(std::move(msg)) {}

        const char* what() const _GLIBCXX_USE_NOEXCEPT override {
                return msg.c_str();
        }

    private:
        std::string msg{};
    };
}
#endif //LAB31PROXY_PARSEEXCEPTION_H
