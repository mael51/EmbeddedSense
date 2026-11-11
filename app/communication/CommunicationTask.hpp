#pragma once
#include "CommunicationManager.hpp"
#include "app/processing/SharedData.hpp"
#include "services/logger/Logger.hpp"
#include <atomic>
#include <thread>
#include <chrono>

class CommunicationTask {
private:
    CommunicationManager& comm;
    SharedData& shared;
    Logger& logger;
    std::atomic<bool> running{false};
    std::thread worker;
    int intervalMs;

    void run();

public:
    CommunicationTask(CommunicationManager& cm, SharedData& data, Logger& log, int intervalMs = 500);
    ~CommunicationTask();
    void start();
    void stop();
    bool isRunning() const;
};