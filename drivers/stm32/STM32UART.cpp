#include "drivers/stm32/STM32UART.hpp"

STM32UART::STM32UART(UART_HandleTypeDef* huart)
    : huart(huart)
{
}

void STM32UART::send(const std::string& data) {
    HAL_UART_Transmit(huart, reinterpret_cast<const uint8_t*>(data.c_str()), data.size(), HAL_MAX_DELAY);
}

std::string STM32UART::receive() {
    if (rxBuffer.empty()) {
        return "";
    }
    std::string data = rxBuffer.front();
    rxBuffer.pop();
    return data;
}

size_t STM32UART::txAvailable() const {
    return 0;  // Hardware gère le buffer TX
}

size_t STM32UART::rxAvailable() const {
    return rxBuffer.size();
}

void STM32UART::onReceiveComplete(uint8_t* data, size_t len) {
    rxBuffer.push(std::string(reinterpret_cast<char*>(data), len));
}