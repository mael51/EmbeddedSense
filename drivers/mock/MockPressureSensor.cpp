#include "drivers/mock/MockPressureSensor.hpp"

MockPressureSensor::MockPressureSensor(unsigned int seed)
    : gen(seed),
      noise(900.0, 1100.0)
{
}

double MockPressureSensor::read() {
    return noise(gen);
}

std::string MockPressureSensor::getName() {
    return "Pressure";
}