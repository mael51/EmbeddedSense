#include <gtest/gtest.h>
#include "app/acquisition/CircularBuffer.hpp"
#include "app/acquisition/SensorManager.hpp"
#include "drivers/mock/MockTemperatureSensor.hpp"
#include "drivers/mock/MockHumiditySensor.hpp"
#include "drivers/mock/MockPressureSensor.hpp"

// ===== Tests CircularBuffer =====

TEST(CircularBufferTest, PushAndGet) {
    CircularBuffer<int, 4> buf;
    buf.push(10);
    buf.push(20);
    buf.push(30);
    EXPECT_EQ(buf.size(), 3u);
    EXPECT_EQ(buf.get(0), 10);
    EXPECT_EQ(buf.get(1), 20);
    EXPECT_EQ(buf.get(2), 30);
}

TEST(CircularBufferTest, Average) {
    CircularBuffer<double, 8> buf;
    buf.push(10.0);
    buf.push(20.0);
    buf.push(30.0);
    EXPECT_DOUBLE_EQ(buf.average(), 20.0);
}

TEST(CircularBufferTest, MinMax) {
    CircularBuffer<int, 8> buf;
    buf.push(5);
    buf.push(2);
    buf.push(8);
    buf.push(1);
    EXPECT_EQ(buf.min(), 1);
    EXPECT_EQ(buf.max(), 8);
}

TEST(CircularBufferTest, Overflow) {
    CircularBuffer<int, 3> buf;
    buf.push(1);
    buf.push(2);
    buf.push(3);
    buf.push(4);  // Écrase 1
    EXPECT_EQ(buf.size(), 3u);
    EXPECT_EQ(buf.get(0), 2);  // Le plus ancien est maintenant 2
    EXPECT_EQ(buf.get(1), 3);
    EXPECT_EQ(buf.get(2), 4);
}

TEST(CircularBufferTest, EmptyBuffer) {
    CircularBuffer<double, 4> buf;
    EXPECT_EQ(buf.size(), 0u);
    EXPECT_DOUBLE_EQ(buf.average(), 0.0);
    EXPECT_DOUBLE_EQ(buf.min(), 0.0);
    EXPECT_DOUBLE_EQ(buf.max(), 0.0);
}

TEST(CircularBufferTest, DoubleType) {
    CircularBuffer<double, 4> buf;
    buf.push(1.5);
    buf.push(2.5);
    EXPECT_DOUBLE_EQ(buf.average(), 2.0);
}

// ===== Tests SensorManager =====

TEST(SensorManagerTest, AddSensors) {
    SensorManager mgr;
    mgr.addSensor(std::make_unique<MockTemperatureSensor>(25.0));
    mgr.addSensor(std::make_unique<MockHumiditySensor>());
    mgr.addSensor(std::make_unique<MockPressureSensor>());
    EXPECT_EQ(mgr.getSensorCount(), 3u);
}

TEST(SensorManagerTest, AcquireAndAvg) {
    SensorManager mgr;
    mgr.addSensor(std::make_unique<MockTemperatureSensor>(25.0));
    mgr.addSensor(std::make_unique<MockHumiditySensor>());
    mgr.addSensor(std::make_unique<MockPressureSensor>());

    for (int i = 0; i < 10; ++i) {
        mgr.acquire();
    }

    double tempAvg = mgr.getTemperatureAvg();
    double humAvg = mgr.getHumidityAvg();
    double presAvg = mgr.getPressureAvg();

    EXPECT_GE(tempAvg, 20.0);
    EXPECT_LE(tempAvg, 30.0);
    EXPECT_GE(humAvg, 0.0);
    EXPECT_LE(humAvg, 100.0);
    EXPECT_GE(presAvg, 900.0);
    EXPECT_LE(presAvg, 1100.0);
}

TEST(SensorManagerTest, SingleAcquisition) {
    SensorManager mgr;
    mgr.addSensor(std::make_unique<MockTemperatureSensor>(25.0));
    mgr.acquire();
    double avg = mgr.getTemperatureAvg();
    EXPECT_GE(avg, 20.0);
    EXPECT_LE(avg, 30.0);
}