//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB32_CONNECTEXCEPTION_H
#define LAB32_CONNECTEXCEPTION_H

#include <exception>
#include <string>

namespace Proxy {

    class ConnectException : public std::exception {
    public:
        explicit ConnectException(std::string msg) : msg(std::move(msg)) {}

        [[nodiscard]] const char *what() const _GLIBCXX_USE_NOEXCEPT override {
            return msg.c_str();
        }

    private:
        std::string msg{};
    };

} // Proxy

#endif //LAB32_CONNECTEXCEPTION_H
