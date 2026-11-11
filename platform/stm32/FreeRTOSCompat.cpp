#include "platform/stm32/FreeRTOSCompat.hpp"
#include "platform/abstraction/IMutex.hpp"
#include <thread>
#include <chrono>

void vTaskDelay(uint32_t ticks) {
    // Sur PC : simule le délai avec sleep_for
    // Sur STM32 : vTaskDelay() de FreeRTOS
    std::this_thread::sleep_for(std::chrono::milliseconds(ticks));
}

void xSemaphoreTake(IMutex& mtx) {
    mtx.lock();
}

void xSemaphoreGive(IMutex& mtx) {
    mtx.unlock();
}