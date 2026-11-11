#include "drivers/mock/MockHumiditySensor.hpp"

MockHumiditySensor::MockHumiditySensor(unsigned int seed)
    : gen(seed),
      noise(0.0, 100.0)
{
}

double MockHumiditySensor::read() {
    return noise(gen);
}

std::string MockHumiditySensor::getName() {
    return "Humidity";
}