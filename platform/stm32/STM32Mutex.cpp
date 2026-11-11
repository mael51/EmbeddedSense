#include "platform/stm32/STM32Mutex.hpp"

STM32Mutex::STM32Mutex() {
    handle = xSemaphoreCreateMutex();
}

STM32Mutex::~STM32Mutex() {
    if (handle != nullptr) {
        vSemaphoreDelete(handle);
    }
}

void STM32Mutex::lock() {
    xSemaphoreTake(handle, portMAX_DELAY);
}

void STM32Mutex::unlock() {
    xSemaphoreGive(handle);
}