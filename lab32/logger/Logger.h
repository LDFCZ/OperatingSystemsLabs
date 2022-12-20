//
// Created by ldfcz on 07.12.22.
//

#ifndef LAB32_LOGGER_H
#define LAB32_LOGGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>

#define TIME_NOW now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec
#define LOG_EVENT(event) Logger::getInstance().logEvent(event)
#define LOG_ERROR_WITH_ERRNO(event) Logger::getInstance().logError(event + std::string(strerror(errno)))
#define LOG_ERROR(event) Logger::getInstance().logError(event)
#define LOGGER_CLOSE Logger::getInstance().closeFile()

class Logger {
public:
    static Logger &getInstance() {
        static Logger instance;
        return instance;
    }

    void openFile() {
        loggerFile.open("log.txt");
    }

    void logEvent(const std::string& str) {
        std::time_t t = std::time(0);   // get time now
        std::tm* now = std::localtime(&t);
        loggerFile << "time: " << TIME_NOW << "   event --->    "  << str << std::endl;
        std::cout << "time: " << TIME_NOW << "   event --->    "  << str << std::endl;
    }

    void logError(const std::string& error) {
        std::time_t t = std::time(nullptr);   // get time now
        std::tm* now = std::localtime(&t);
        loggerFile << "time: " << TIME_NOW << "   ERROR --->    "  << error << std::endl;
        std::cout << "time: " << TIME_NOW << "   ERROR --->    "  << error << std::endl;
    }

    void closeFile() {
        loggerFile.close();
    }

private:
    Logger() {
        openFile();
    }

    ~Logger() {
        loggerFile.close();
    }

    std::ofstream loggerFile;
};


#endif //LAB32_LOGGER_H
