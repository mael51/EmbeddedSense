#include "app/processing/SharedData.hpp"

void SharedData::update(const SensorReading& data) {
    std::lock_guard<std::mutex> lock(mtx);
    latest = data;
    dataReady = true;
    ++updateCount;
}

SensorReading SharedData::getLatest() const {
    std::lock_guard<std::mutex> lock(mtx);
    return latest;
}

bool SharedData::isReady() const {
    std::lock_guard<std::mutex> lock(mtx);
    return dataReady;
}

uint64_t SharedData::getUpdateCount() const {
    return updateCount.load();
}