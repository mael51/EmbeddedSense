#pragma once
#include <cstdint>
#include "platform/abstraction/IMutex.hpp"

// Stubs pour l'API FreeRTOS — compilent sur PC, remplacés par la vraie FreeRTOS sur STM32
void vTaskDelay(uint32_t ticks);
void xSemaphoreTake(IMutex& mtx);
void xSemaphoreGive(IMutex& mtx);