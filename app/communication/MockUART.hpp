#pragma once
#include <queue>
#include <string>

class MockUART {
private:
    std::queue<std::string> txBuffer;   // Envoi
    std::queue<std::string> rxBuffer;   // Réception
    int baudRate;

public:
    MockUART(int baudRate = 115200);
    void send(const std::string& data);
    std::string receive();
    size_t txAvailable() const;
    size_t rxAvailable() const;
    void feedRx(const std::string& data);
};