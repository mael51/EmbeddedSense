#pragma once
#include "platform/abstraction/IMutex.hpp"
#include <mutex>

class HostMutex : public IMutex {
private:
    std::mutex mtx;

public:
    void lock() override;
    void unlock() override;
};