//
// Created by ldfcz on 12.12.22.
//

#ifndef LAB32_CASHCONTEXTEXCEPTION_H
#define LAB32_CASHCONTEXTEXCEPTION_H

#include <exception>
#include <string>


namespace Proxy {

    class CashContextException : public std::exception {
    public:
        explicit CashContextException(std::string msg) : msg(std::move(msg)) {}

        [[nodiscard]] const char *what() const _GLIBCXX_USE_NOEXCEPT override {
                return msg.c_str();
        }

    private:
        std::string msg{};
    };

} // Proxy

#endif //LAB32_CASHCONTEXTEXCEPTION_H
