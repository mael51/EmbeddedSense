#include "app/communication/CommunicationTask.hpp"

CommunicationTask::CommunicationTask(CommunicationManager& cm, SharedData& data, Logger& log, int intervalMs)
    : comm(cm),
      shared(data),
      logger(log),
      intervalMs(intervalMs)
{
}

CommunicationTask::~CommunicationTask() {
    stop();
}

void CommunicationTask::run() {
    while (running.load()) {
        if (shared.isReady()) {
            SensorReading reading = shared.getLatest();
            std::string msg = "Temp=" + std::to_string(reading.temperature) +
                              " Hum=" + std::to_string(reading.humidity) +
                              " Pres=" + std::to_string(reading.pressure);
            comm.sendLog(LogLevel::INFO, msg);
        }

        comm.processIncoming();

        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
}

void CommunicationTask::start() {
    if (!running.load()) {
        running.store(true);
        worker = std::thread(&CommunicationTask::run, this);
    }
}

void CommunicationTask::stop() {
    if (running.load()) {
        running.store(false);
        if (worker.joinable()) {
            worker.join();
        }
    }
}

bool CommunicationTask::isRunning() const {
    return running.load();
}