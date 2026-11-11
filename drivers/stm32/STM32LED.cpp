#include "drivers/stm32/STM32LED.hpp"

STM32LED::STM32LED(GPIO_TypeDef* port, uint16_t pin)
    : port(port),
      pin(pin)
{
}

void STM32LED::on() {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

void STM32LED::off() {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

void STM32LED::toggle() {
    HAL_GPIO_TogglePin(port, pin);
}

bool STM32LED::isOn() const {
    return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET;
}