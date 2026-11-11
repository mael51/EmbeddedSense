#include "drivers/stm32/STM32HumiditySensor.hpp"

STM32HumiditySensor::STM32HumiditySensor(ADC_HandleTypeDef* hadc, uint32_t channel)
    : hadc(hadc),
      channel(channel)
{
}

double STM32HumiditySensor::read() {
    ADC_ChannelConfTypeDef sConfig = {};
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(hadc, &sConfig);

    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
    uint32_t adcValue = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);

    // Convertir en % d'humidité (exemple : capteur HIH-4000)
    double voltage = (adcValue * 3.3) / 4095.0;
    double humidity = (voltage - 0.826) / 0.0315;

    if (humidity < 0.0) humidity = 0.0;
    if (humidity > 100.0) humidity = 100.0;

    return humidity;
}

std::string STM32HumiditySensor::getName() {
    return "Humidity";
}