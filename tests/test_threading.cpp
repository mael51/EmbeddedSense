#include <gtest/gtest.h>
#include "app/processing/SharedData.hpp"
#include "app/acquisition/SensorManager.hpp"
#include "drivers/mock/MockTemperatureSensor.hpp"
#include "app/acquisition/AcquisitionTask.hpp"
#include "app/processing/ProcessingTask.hpp"
#include "app/control/StateMachine.hpp"
#include <thread>
#include <vector>
#include <atomic>

// ===== Tests SharedData =====

TEST(SharedDataTest, UpdateAndGet) {
    SharedData shared;
    SensorReading r1{25.0, 50.0, 1013.0, 1000};
    shared.update(r1);
    SensorReading r2 = shared.getLatest();
    EXPECT_DOUBLE_EQ(r2.temperature, 25.0);
    EXPECT_DOUBLE_EQ(r2.humidity, 50.0);
    EXPECT_DOUBLE_EQ(r2.pressure, 1013.0);
    EXPECT_TRUE(shared.isReady());
}

TEST(SharedDataTest, UpdateCount) {
    SharedData shared;
    EXPECT_EQ(shared.getUpdateCount(), 0u);
    shared.update({25.0, 50.0, 1013.0, 1000});
    EXPECT_EQ(shared.getUpdateCount(), 1u);
    shared.update({26.0, 51.0, 1014.0, 2000});
    EXPECT_EQ(shared.getUpdateCount(), 2u);
}

TEST(SharedDataTest, ThreadSafety) {
    SharedData shared;
    std::atomic<int> errors{0};

    // 10 threads qui écrivent
    std::vector<std::thread> writers;
    for (int i = 0; i < 10; ++i) {
        writers.emplace_back([&shared, &errors, i]() {
            for (int j = 0; j < 100; ++j) {
                try {
                    shared.update({static_cast<double>(i), static_cast<double>(j), 1000.0, static_cast<uint64_t>(j)});
                } catch (...) {
                    ++errors;
                }
            }
        });
    }

    // 10 threads qui lisent
    std::vector<std::thread> readers;
    for (int i = 0; i < 10; ++i) {
        readers.emplace_back([&shared, &errors]() {
            for (int j = 0; j < 100; ++j) {
                try {
                    SensorReading r = shared.getLatest();
                    (void)r;
                } catch (...) {
                    ++errors;
                }
            }
        });
    }

    for (auto& t : writers) t.join();
    for (auto& t : readers) t.join();

    EXPECT_EQ(errors.load(), 0);
}

TEST(SharedDataTest, NoRaceCondition) {
    SharedData shared;
    std::atomic<bool> running{true};

    std::thread writer([&shared, &running]() {
        int i = 0;
        while (running.load()) {
            shared.update({static_cast<double>(i), 50.0, 1000.0, static_cast<uint64_t>(i)});
            ++i;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });

    std::thread reader([&shared, &running]() {
        for (int i = 0; i < 1000; ++i) {
            SensorReading r = shared.getLatest();
            // Vérifie que la température est un nombre valide (pas corrompu)
            EXPECT_FALSE(std::isnan(r.temperature));
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    running.store(false);
    writer.join();
    reader.join();
}

// ===== Tests AcquisitionTask =====

TEST(AcquisitionTaskTest, StartStop) {
    SensorManager mgr;
    mgr.addSensor(std::make_unique<MockTemperatureSensor>(25.0));
    SharedData shared;
    AcquisitionTask task(mgr, shared);

    EXPECT_FALSE(task.isRunning());
    task.start();
    EXPECT_TRUE(task.isRunning());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    EXPECT_GE(shared.getUpdateCount(), 1u);

    task.stop();
    EXPECT_FALSE(task.isRunning());
}

TEST(AcquisitionTaskTest, DataAcquired) {
    SensorManager mgr;
    mgr.addSensor(std::make_unique<MockTemperatureSensor>(25.0));
    SharedData shared;
    AcquisitionTask task(mgr, shared, 50);

    task.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    task.stop();

    EXPECT_TRUE(shared.isReady());
    SensorReading r = shared.getLatest();
    EXPECT_GE(r.temperature, 20.0);
    EXPECT_LE(r.temperature, 30.0);
}

// ===== Tests ProcessingTask =====

TEST(ProcessingTaskTest, StartStop) {
    SharedData shared;
    StateMachine fsm;
    ProcessingTask task(shared, fsm);

    EXPECT_FALSE(task.isRunning());
    task.start();
    EXPECT_TRUE(task.isRunning());
    task.stop();
    EXPECT_FALSE(task.isRunning());
}