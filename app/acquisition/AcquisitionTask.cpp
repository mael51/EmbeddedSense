#include "app/acquisition/AcquisitionTask.hpp"

AcquisitionTask::AcquisitionTask(SensorManager& mgr, SharedData& data, int intervalMs)
    : sensorMgr(mgr),
      shared(data),
      intervalMs(intervalMs)
{
}

AcquisitionTask::~AcquisitionTask() {
    stop();
}

void AcquisitionTask::run() {
    while (running.load()) {
        sensorMgr.acquire();

        SensorReading reading;
        reading.temperature = sensorMgr.getTemperatureAvg();
        reading.humidity = sensorMgr.getHumidityAvg();
        reading.pressure = sensorMgr.getPressureAvg();
        reading.timestamp = static_cast<uint64_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count());

        shared.update(reading);

        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
}

void AcquisitionTask::start() {
    if (!running.load()) {
        running.store(true);
        worker = std::thread(&AcquisitionTask::run, this);
    }
}

void AcquisitionTask::stop() {
    if (running.load()) {
        running.store(false);
        if (worker.joinable()) {
            worker.join();
        }
    }
}

bool AcquisitionTask::isRunning() const {
    return running.load();
}