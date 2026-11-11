#include "platform/stm32/STM32Task.hpp"

STM32Task::STM32Task(std::function<void()> func, const char* name, uint16_t stackSize, UBaseType_t priority)
    : taskFunc(std::move(func)),
      name(name),
      stackSize(stackSize),
      priority(priority)
{
}

STM32Task::~STM32Task() {
    stop();
}

void STM32Task::taskWrapper(void* param) {
    STM32Task* self = static_cast<STM32Task*>(param);
    if (self->taskFunc) {
        self->taskFunc();
    }
    vTaskDelete(nullptr);
}

void STM32Task::start() {
    if (handle == nullptr) {
        xTaskCreate(taskWrapper, name, stackSize, this, priority, &handle);
    }
}

void STM32Task::stop() {
    if (handle != nullptr) {
        vTaskDelete(handle);
        handle = nullptr;
    }
}

bool STM32Task::isRunning() const {
    return handle != nullptr;
}