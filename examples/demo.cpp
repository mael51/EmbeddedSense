#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "app/acquisition/SensorManager.hpp"
#include "drivers/mock/MockHumiditySensor.hpp"
#include "drivers/mock/MockPressureSensor.hpp"
#include "drivers/mock/MockTemperatureSensor.hpp"
#include "services/logger/Logger.hpp"

int main() {
    SensorManager sensors;
    sensors.addSensor(std::make_unique<MockTemperatureSensor>(25.0, 42));
    sensors.addSensor(std::make_unique<MockHumiditySensor>(42));
    sensors.addSensor(std::make_unique<MockPressureSensor>(42));

    Logger log(LogLevel::INFO);
    log.log(LogLevel::INFO, "EmbeddedSense demo — démarrage");

    for (int i = 0; i < 10; ++i) {
        sensors.acquire();

        double t = sensors.getTemperatureAvg();
        double h = sensors.getHumidityAvg();
        double p = sensors.getPressureAvg();

        std::cout << "[cycle " << i << "] "
                  << "T=" << t << " C  "
                  << "H=" << h << " %  "
                  << "P=" << p << " hPa"
                  << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    log.log(LogLevel::INFO, "EmbeddedSense demo — fin");
    return 0;
}