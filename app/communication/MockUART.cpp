#include "app/communication/MockUART.hpp"

MockUART::MockUART(int baudRate)
    : baudRate(baudRate)
{
}

void MockUART::send(const std::string& data) {
    txBuffer.push(data);
}

std::string MockUART::receive() {
    if (rxBuffer.empty()) {
        return "";
    }
    std::string data = rxBuffer.front();
    rxBuffer.pop();
    return data;
}

size_t MockUART::txAvailable() const {
    return txBuffer.size();
}

size_t MockUART::rxAvailable() const {
    return rxBuffer.size();
}

void MockUART::feedRx(const std::string& data) {
    rxBuffer.push(data);
}