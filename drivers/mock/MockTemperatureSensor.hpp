#pragma once
#include "drivers/interfaces/ISensor.hpp"
#include <random>

class MockTemperatureSensor : public ISensor {
private:
    double baseTemp;
    std::mt19937 gen;
    std::uniform_real_distribution<double> noise;

public:
    MockTemperatureSensor(double baseTemp = 25.0, unsigned int seed = 42);
    double read() override;
    std::string getName() override;
    SensorType getType() const override { return SensorType::TEMPERATURE; }
};