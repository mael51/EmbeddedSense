#pragma once
#include "stm32f4xx_hal.h"
#include <cstdint>

class STM32LED {
private:
    GPIO_TypeDef* port;
    uint16_t pin;

public:
    STM32LED(GPIO_TypeDef* port, uint16_t pin);
    void on();
    void off();
    void toggle();
    bool isOn() const;
};