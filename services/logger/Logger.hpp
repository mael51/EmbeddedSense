#pragma once
#include "LogLevel.hpp"
#include <vector>
#include <string>
#include <ctime>

class Logger {
private:
    std::vector<std::string> logBuffer;
    LogLevel minLevel;

    std::string levelToString(LogLevel level) const;
    std::string getCurrentTime() const;

public:
    Logger(LogLevel minLevel = LogLevel::INFO);
    void log(LogLevel level, const std::string& message);
    std::string format(LogLevel level, const std::string& msg) const;
    const std::vector<std::string>& getLogs() const;
    size_t getLogCount() const;
    void clear();
    LogLevel getMinLevel() const;
};