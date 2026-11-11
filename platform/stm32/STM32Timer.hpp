#pragma once
#include "platform/abstraction/ITimer.hpp"
#include "FreeRTOS.h"
#include "timers.h"
#include <cstdint>

class STM32Timer : public ITimer {
private:
    TimerHandle_t handle = nullptr;
    uint32_t periodMs = 0;
    bool expired = false;

    static void timerCallback(TimerHandle_t xTimer);

public:
    STM32Timer();
    ~STM32Timer();
    void start(uint32_t periodMs) override;
    void stop() override;
    bool hasExpired() const override;
};