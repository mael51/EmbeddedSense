#include "services/logger/Logger.hpp"
#include <sstream>
#include <iomanip>

Logger::Logger(LogLevel minLevel)
    : minLevel(minLevel)
{
}

std::string Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default:             return "UNKNOWN";
    }
}

std::string Logger::getCurrentTime() const {
    std::time_t now = std::time(nullptr);
    std::tm* tm = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void Logger::log(LogLevel level, const std::string& message) {
    if (static_cast<uint8_t>(level) < static_cast<uint8_t>(minLevel)) {
        return;  // Niveau trop bas, on ignore
    }
    std::string formatted = format(level, message);
    logBuffer.push_back(formatted);
}

std::string Logger::format(LogLevel level, const std::string& msg) const {
    std::ostringstream oss;
    oss << "[" << getCurrentTime() << "] "
        << "[" << levelToString(level) << "] "
        << msg;
    return oss.str();
}

const std::vector<std::string>& Logger::getLogs() const {
    return logBuffer;
}

size_t Logger::getLogCount() const {
    return logBuffer.size();
}

void Logger::clear() {
    logBuffer.clear();
}

LogLevel Logger::getMinLevel() const {
    return minLevel;
}