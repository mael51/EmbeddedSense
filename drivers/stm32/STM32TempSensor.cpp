#include "drivers/stm32/STM32TempSensor.hpp"

STM32TempSensor::STM32TempSensor(ADC_HandleTypeDef* hadc, uint32_t channel, double offset)
    : hadc(hadc),
      channel(channel),
      calibrationOffset(offset)
{
}

double STM32TempSensor::read() {
    // Configurer le canal ADC
    ADC_ChannelConfTypeDef sConfig = {};
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(hadc, &sConfig);

    // Lancer la conversion
    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
    uint32_t adcValue = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);

    // Convertir en température (exemple : capteur LM35, 10mV/°C)
    // ADC 12 bits, Vref = 3.3V
    double voltage = (adcValue * 3.3) / 4095.0;
    double temperature = voltage * 100.0 + calibrationOffset;

    return temperature;
}

std::string STM32TempSensor::getName() {
    return "Temperature";
}