#include "platform/host/HostMutex.hpp"

void HostMutex::lock() {
    mtx.lock();
}

void HostMutex::unlock() {
    mtx.unlock();
}