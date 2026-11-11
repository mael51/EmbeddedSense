#pragma once
#include "SensorManager.hpp"
#include "app/processing/SharedData.hpp"
#include "app/control/StateMachine.hpp"
#include <atomic>
#include <thread>
#include <chrono>

class AcquisitionTask {
private:
    SensorManager& sensorMgr;
    SharedData& shared;
    std::atomic<bool> running{false};
    std::thread worker;
    int intervalMs;

    void run();

public:
    AcquisitionTask(SensorManager& mgr, SharedData& data, int intervalMs = 100);
    ~AcquisitionTask();
    void start();
    void stop();
    bool isRunning() const;
};