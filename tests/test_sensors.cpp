#include <gtest/gtest.h>
#include "drivers/interfaces/ISensor.hpp"
#include "drivers/mock/MockTemperatureSensor.hpp"
#include "drivers/mock/MockHumiditySensor.hpp"
#include "drivers/mock/MockPressureSensor.hpp"

TEST(SensorTest, TemperatureName) {
    MockTemperatureSensor sensor;
    EXPECT_EQ(sensor.getName(), "Temperature");
}

TEST(SensorTest, TemperatureRange) {
    MockTemperatureSensor sensor(25.0);
    for (int i = 0; i < 1000; ++i) {
        double value = sensor.read();
        EXPECT_GE(value, 20.0);
        EXPECT_LE(value, 30.0);
    }
}

TEST(SensorTest, HumidityName) {
    MockHumiditySensor sensor;
    EXPECT_EQ(sensor.getName(), "Humidity");
}

TEST(SensorTest, HumidityRange) {
    MockHumiditySensor sensor;
    for (int i = 0; i < 1000; ++i) {
        double value = sensor.read();
        EXPECT_GE(value, 0.0);
        EXPECT_LE(value, 100.0);
    }
}

TEST(SensorTest, PressureName) {
    MockPressureSensor sensor;
    EXPECT_EQ(sensor.getName(), "Pressure");
}

TEST(SensorTest, PressureRange) {
    MockPressureSensor sensor;
    for (int i = 0; i < 1000; ++i) {
        double value = sensor.read();
        EXPECT_GE(value, 900.0);
        EXPECT_LE(value, 1100.0);
    }
}

TEST(SensorTest, Polymorphism) {
    ISensor* sensor = new MockTemperatureSensor(25.0);
    EXPECT_EQ(sensor->getName(), "Temperature");
    double value = sensor->read();
    EXPECT_GE(value, 20.0);
    EXPECT_LE(value, 30.0);
    delete sensor;
}