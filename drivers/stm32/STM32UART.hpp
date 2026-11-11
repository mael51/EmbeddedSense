#pragma once
#include "stm32f4xx_hal.h"
#include <string>
#include <queue>

class STM32UART {
private:
    UART_HandleTypeDef* huart;
    std::queue<std::string> rxBuffer;

public:
    STM32UART(UART_HandleTypeDef* huart);
    void send(const std::string& data);
    std::string receive();
    size_t txAvailable() const;
    size_t rxAvailable() const;
    void onReceiveComplete(uint8_t* data, size_t len);
};