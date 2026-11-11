#include "app/communication/CommunicationManager.hpp"

CommunicationManager::CommunicationManager(MockUART& uart, Logger& logger)
    : uart(uart),
      logger(logger)
{
}

void CommunicationManager::sendLog(LogLevel level, const std::string& message) {
    logger.log(level, message);
    uart.send(logger.format(level, message));
}

void CommunicationManager::processIncoming() {
    while (uart.rxAvailable() > 0) {
        std::string command = uart.receive();
        std::string response = handleCommand(command);
        if (!response.empty()) {
            uart.send(response);
        }
    }
}

std::string CommunicationManager::handleCommand(const std::string& command) {
    if (command == "STATUS") {
        return "OK:STATUS";
    }
    if (command == "RESET") {
        return "OK:RESET";
    }
    if (command.rfind("LOG_LEVEL ", 0) == 0) {
        return "OK:LOG_LEVEL_CHANGED";
    }
    return "ERR:UNKNOWN_CMD";
}