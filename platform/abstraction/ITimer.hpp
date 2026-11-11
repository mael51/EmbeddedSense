#pragma once
#include <cstdint>

class ITimer {
public:
    virtual ~ITimer() = default;
    virtual void start(uint32_t periodMs) = 0;
    virtual void stop() = 0;
    virtual bool hasExpired() const = 0;
};