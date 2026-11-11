#include "drivers/mock/MockTemperatureSensor.hpp"

MockTemperatureSensor::MockTemperatureSensor(double baseTemp, unsigned int seed)
    : baseTemp(baseTemp),
      gen(seed),
      noise(-5.0, 5.0)
{
}

double MockTemperatureSensor::read() {
    return baseTemp + noise(gen);
}

std::string MockTemperatureSensor::getName() {
    return "Temperature";
}