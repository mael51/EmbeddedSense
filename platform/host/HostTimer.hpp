#pragma once
#include "platform/abstraction/ITimer.hpp"
#include <chrono>
#include <atomic>

class HostTimer : public ITimer {
private:
    std::chrono::steady_clock::time_point startTime;
    uint32_t periodMs = 0;
    std::atomic<bool> running{false};
    mutable std::atomic<bool> expired{false};

public:
    void start(uint32_t periodMs) override;
    void stop() override;
    bool hasExpired() const override;
};