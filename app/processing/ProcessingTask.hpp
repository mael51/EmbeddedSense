#pragma once
#include "SharedData.hpp"
#include "app/control/StateMachine.hpp"
#include <atomic>
#include <thread>
#include <chrono>

class ProcessingTask {
private:
    SharedData& shared;
    StateMachine& fsm;
    std::atomic<bool> running{false};
    std::thread worker;
    int intervalMs;

    void run();

public:
    ProcessingTask(SharedData& data, StateMachine& fsm, int intervalMs = 200);
    ~ProcessingTask();
    void start();
    void stop();
    bool isRunning() const;
};