#include "platform/host/HostTask.hpp"

HostTask::HostTask(std::function<void()> func)
    : taskFunc(std::move(func))
{
}

HostTask::~HostTask() {
    stop();
}

void HostTask::start() {
    if (!running.load()) {
        running.store(true);
        worker = std::thread([this]() {
            while (running.load()) {
                if (taskFunc) {
                    taskFunc();
                }
            }
        });
    }
}

void HostTask::stop() {
    if (running.load()) {
        running.store(false);
        if (worker.joinable()) {
            worker.join();
        }
    }
}

bool HostTask::isRunning() const {
    return running.load();
}