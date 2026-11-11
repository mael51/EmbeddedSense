#pragma once
#include "platform/abstraction/IMutex.hpp"
#include "FreeRTOS.h"
#include "semphr.h"

class STM32Mutex : public IMutex {
private:
    SemaphoreHandle_t handle;

public:
    STM32Mutex();
    ~STM32Mutex();
    void lock() override;
    void unlock() override;
};