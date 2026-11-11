#pragma once
#include "platform/abstraction/ITask.hpp"
#include <atomic>
#include <thread>
#include <functional>

class HostTask : public ITask {
private:
    std::atomic<bool> running{false};
    std::thread worker;
    std::function<void()> taskFunc;

public:
    explicit HostTask(std::function<void()> func);
    ~HostTask();
    void start() override;
    void stop() override;
    bool isRunning() const override;
};