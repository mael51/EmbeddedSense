#pragma once
#include "drivers/interfaces/ISensor.hpp"
#include "stm32f4xx_hal.h"
#include <cstdint>

class STM32HumiditySensor : public ISensor {
private:
    ADC_HandleTypeDef* hadc;
    uint32_t channel;

public:
    STM32HumiditySensor(ADC_HandleTypeDef* hadc, uint32_t channel);
    double read() override;
    std::string getName() override;
    SensorType getType() const override { return SensorType::HUMIDITY; }
};