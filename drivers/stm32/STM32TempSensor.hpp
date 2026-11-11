#pragma once
#include "drivers/interfaces/ISensor.hpp"
#include "stm32f4xx_hal.h"
#include <cstdint>

class STM32TempSensor : public ISensor {
private:
    ADC_HandleTypeDef* hadc;
    uint32_t channel;
    double calibrationOffset;

public:
    STM32TempSensor(ADC_HandleTypeDef* hadc, uint32_t channel, double offset = 0.0);
    double read() override;
    std::string getName() override;
    SensorType getType() const override { return SensorType::TEMPERATURE; }
};