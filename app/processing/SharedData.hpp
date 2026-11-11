#pragma once
#include <mutex>
#include <cstdint>
#include <atomic>

struct SensorReading {
    double temperature;
    double humidity;
    double pressure;
    uint64_t timestamp;
};

class SharedData {
private:
    mutable std::mutex mtx;
    SensorReading latest;
    bool dataReady = false;
    std::atomic<uint64_t> updateCount{0};

public:
    void update(const SensorReading& data);
    SensorReading getLatest() const;
    bool isReady() const;
    uint64_t getUpdateCount() const;
};