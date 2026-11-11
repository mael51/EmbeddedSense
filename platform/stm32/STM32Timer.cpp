#include "platform/stm32/STM32Timer.hpp"

void STM32Timer::timerCallback(TimerHandle_t xTimer) {
    STM32Timer* self = static_cast<STM32Timer*>(pvTimerGetTimerID(xTimer));
    if (self) {
        self->expired = true;
    }
}

STM32Timer::STM32Timer() {
}

STM32Timer::~STM32Timer() {
    stop();
}

void STM32Timer::start(uint32_t periodMs) {
    this->periodMs = periodMs;
    this->expired = false;

    if (handle == nullptr) {
        handle = xTimerCreate("STM32Timer", pdMS_TO_TICKS(periodMs), pdFALSE, this, timerCallback);
    }
    xTimerStart(handle, 0);
}

void STM32Timer::stop() {
    if (handle != nullptr) {
        xTimerStop(handle, 0);
        xTimerDelete(handle, 0);
        handle = nullptr;
    }
    expired = false;
}

bool STM32Timer::hasExpired() const {
    return expired;
}