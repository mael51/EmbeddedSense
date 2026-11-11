#include "platform/host/HostTimer.hpp"

void HostTimer::start(uint32_t periodMs) {
    this->periodMs = periodMs;
    startTime = std::chrono::steady_clock::now();
    running.store(true);
    expired.store(false);
}

void HostTimer::stop() {
    running.store(false);
    expired.store(false);
}

bool HostTimer::hasExpired() const {
    if (!running.load()) {
        return expired.load();
    }
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
    if (elapsed >= static_cast<long long>(periodMs)) {
        expired.store(true);
        return true;
    }
    return false;
}