#include <gtest/gtest.h>
#include "platform/abstraction/ITask.hpp"
#include "platform/abstraction/IMutex.hpp"
#include "platform/abstraction/ITimer.hpp"
#include "platform/host/HostTask.hpp"
#include "platform/host/HostMutex.hpp"
#include "platform/host/HostTimer.hpp"
#include "platform/stm32/FreeRTOSCompat.hpp"
#include <thread>
#include <atomic>

// ===== Tests ITask / HostTask =====

TEST(HostTaskTest, StartStop) {
    std::atomic<bool> flag{false};
    HostTask task([&flag]() {
        flag.store(true);
    });

    EXPECT_FALSE(task.isRunning());
    task.start();
    EXPECT_TRUE(task.isRunning());

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_TRUE(flag.load());

    task.stop();
    EXPECT_FALSE(task.isRunning());
}

TEST(HostTaskTest, DestructorStops) {
    std::atomic<bool> running{true};
    {
        HostTask task([&running]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        });
        task.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }  // Destructeur appelle stop()
    // Si on arrive ici sans crash, c'est OK
    SUCCEED();
}

// ===== Tests IMutex / HostMutex =====

TEST(HostMutexTest, LockUnlock) {
    HostMutex mtx;
    mtx.lock();
    mtx.unlock();
    SUCCEED();
}

TEST(HostMutexTest, ThreadSafety) {
    HostMutex mtx;
    std::atomic<int> counter{0};
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&mtx, &counter]() {
            for (int j = 0; j < 100; ++j) {
                mtx.lock();
                ++counter;
                mtx.unlock();
            }
        });
    }

    for (auto& t : threads) t.join();
    EXPECT_EQ(counter.load(), 1000);
}

// ===== Tests ITimer / HostTimer =====

TEST(HostTimerTest, NotExpiredInitially) {
    HostTimer timer;
    timer.start(100);
    EXPECT_FALSE(timer.hasExpired());
    timer.stop();
}

TEST(HostTimerTest, ExpiresAfterPeriod) {
    HostTimer timer;
    timer.start(50);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_TRUE(timer.hasExpired());
    timer.stop();
}

TEST(HostTimerTest, StopResets) {
    HostTimer timer;
    timer.start(50);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_TRUE(timer.hasExpired());
    timer.stop();
    EXPECT_FALSE(timer.hasExpired());
}

// ===== Tests FreeRTOSCompat =====

TEST(FreeRTOSCompatTest, VTaskDelay) {
    auto start = std::chrono::steady_clock::now();
    vTaskDelay(50);
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    EXPECT_GE(elapsed, 40);  // Au moins 40ms (marge)
}

TEST(FreeRTOSCompatTest, XSemaphoreTakeGive) {
    HostMutex mtx;
    xSemaphoreTake(mtx);
    xSemaphoreGive(mtx);
    SUCCEED();
}

TEST(FreeRTOSCompatTest, CompileCheck) {
    // Vérifie que les fonctions FreeRTOSCompat compilent
    IMutex* mtx = new HostMutex();
    xSemaphoreTake(*mtx);
    xSemaphoreGive(*mtx);
    delete mtx;
    SUCCEED();
}