#include "app/acquisition/SensorManager.hpp"

void SensorManager::addSensor(std::unique_ptr<ISensor> sensor) {
    sensors.push_back(std::move(sensor));
}

void SensorManager::acquire() {
    for (auto& sensor : sensors) {
        double value = sensor->read();
        switch (sensor->getType()) {
            case SensorType::TEMPERATURE:
                tempBuffer.push(value);
                break;
            case SensorType::HUMIDITY:
                humidityBuffer.push(value);
                break;
            case SensorType::PRESSURE:
                pressureBuffer.push(value);
                break;
            default:
                break;
        }
    }
}

double SensorManager::getTemperatureAvg() const {
    return tempBuffer.average();
}

double SensorManager::getHumidityAvg() const {
    return humidityBuffer.average();
}

double SensorManager::getPressureAvg() const {
    return pressureBuffer.average();
}

size_t SensorManager::getSensorCount() const {
    return sensors.size();
}