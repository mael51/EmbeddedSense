#pragma once
#include "MockUART.hpp"
#include "services/logger/Logger.hpp"
#include "services/logger/LogLevel.hpp"
#include <string>

class CommunicationManager {
private:
    MockUART& uart;
    Logger& logger;

public:
    CommunicationManager(MockUART& uart, Logger& logger);
    void sendLog(LogLevel level, const std::string& message);
    void processIncoming();
    std::string handleCommand(const std::string& command);
};