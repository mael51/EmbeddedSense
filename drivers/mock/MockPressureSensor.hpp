#pragma once
#include "drivers/interfaces/ISensor.hpp"
#include <random>

class MockPressureSensor : public ISensor {
private:
    std::mt19937 gen;
    std::uniform_real_distribution<double> noise;

public:
    MockPressureSensor(unsigned int seed = 42);
    double read() override;
    std::string getName() override;
    SensorType getType() const override { return SensorType::PRESSURE; }
};