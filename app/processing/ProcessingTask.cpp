#include "app/processing/ProcessingTask.hpp"

ProcessingTask::ProcessingTask(SharedData& data, StateMachine& fsm, int intervalMs)
    : shared(data),
      fsm(fsm),
      intervalMs(intervalMs)
{
}

ProcessingTask::~ProcessingTask() {
    stop();
}

void ProcessingTask::run() {
    while (running.load()) {
        if (shared.isReady()) {
            SensorReading reading = shared.getLatest();

            // Vérifier les seuils
            if (reading.temperature > 30.0 || reading.temperature < 10.0) {
                fsm.handleEvent(Event::THRESHOLD_EXCEEDED);
            }
            // Si FSM en erreur, on pourrait notifier ici
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
}

void ProcessingTask::start() {
    if (!running.load()) {
        running.store(true);
        worker = std::thread(&ProcessingTask::run, this);
    }
}

void ProcessingTask::stop() {
    if (running.load()) {
        running.store(false);
        if (worker.joinable()) {
            worker.join();
        }
    }
}

bool ProcessingTask::isRunning() const {
    return running.load();
}