#pragma once
#include <string>

enum class SensorType {
    TEMPERATURE,
    HUMIDITY,
    PRESSURE,
    UNKNOWN
};

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual double read() = 0;
    virtual std::string getName() = 0;
    virtual SensorType getType() const = 0;
};