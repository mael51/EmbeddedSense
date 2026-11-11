#pragma once
#include "platform/abstraction/ITask.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include <functional>

class STM32Task : public ITask {
private:
    TaskHandle_t handle = nullptr;
    std::function<void()> taskFunc;
    const char* name;
    uint16_t stackSize;
    UBaseType_t priority;

    static void taskWrapper(void* param);

public:
    STM32Task(std::function<void()> func, const char* name, uint16_t stackSize = 256, UBaseType_t priority = 1);
    ~STM32Task();
    void start() override;
    void stop() override;
    bool isRunning() const override;
};