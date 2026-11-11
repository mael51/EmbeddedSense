#pragma once
#include "drivers/interfaces/ISensor.hpp"
#include "CircularBuffer.hpp"
#include <vector>
#include <memory>
#include <string>

class SensorManager {
private:
    std::vector<std::unique_ptr<ISensor>> sensors;
    CircularBuffer<double, 64> tempBuffer;
    CircularBuffer<double, 64> humidityBuffer;
    CircularBuffer<double, 64> pressureBuffer;

public:
    void addSensor(std::unique_ptr<ISensor> sensor);
    void acquire();
    double getTemperatureAvg() const;
    double getHumidityAvg() const;
    double getPressureAvg() const;
    size_t getSensorCount() const;
};