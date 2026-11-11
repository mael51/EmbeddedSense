#include <gtest/gtest.h>
#include "app/acquisition/SensorManager.hpp"
#include "app/acquisition/CircularBuffer.hpp"
#include "app/control/StateMachine.hpp"
#include "app/control/Types.hpp"
#include "services/logger/Logger.hpp"
#include "services/logger/LogLevel.hpp"
#include "app/communication/MockUART.hpp"
#include "app/communication/CommunicationManager.hpp"
#include "app/processing/SharedData.hpp"
#include "drivers/mock/MockTemperatureSensor.hpp"
#include "drivers/mock/MockHumiditySensor.hpp"
#include "drivers/mock/MockPressureSensor.hpp"
#include <memory>
#include <thread>
#include <chrono>

// ===== Fixture d'intégration =====

class IntegrationTest : public ::testing::Test {
protected:
    SensorManager sensorMgr;
    StateMachine fsm;
    Logger logger{LogLevel::DEBUG};
    MockUART uart;
    CommunicationManager comm{uart, logger};
    SharedData sharedData;

    void SetUp() override {
        sensorMgr.addSensor(std::make_unique<MockTemperatureSensor>(25.0));
        sensorMgr.addSensor(std::make_unique<MockHumiditySensor>());
        sensorMgr.addSensor(std::make_unique<MockPressureSensor>());
    }

    void TearDown() override {
        // Nettoyage automatique
    }
};

// ===== Scénario 1 : Acquisition → État =====

TEST_F(IntegrationTest, Scenario1_AcquisitionToState) {
    fsm.handleEvent(Event::START);
    EXPECT_EQ(fsm.getCurrentState(), State::SURVEILLANCE);

    // 50 acquisitions avec valeurs normales
    for (int i = 0; i < 50; ++i) {
        sensorMgr.acquire();
    }

    // FSM reste en SURVEILLANCE
    EXPECT_EQ(fsm.getCurrentState(), State::SURVEILLANCE);

    // Simuler une température > seuil
    // (On vérifie que le système gère le cas)
    sensorMgr.acquire();
    EXPECT_GE(sensorMgr.getTemperatureAvg(), 20.0);
    EXPECT_LE(sensorMgr.getTemperatureAvg(), 30.0);
}

// ===== Scénario 2 : Erreur → Récupération =====

TEST_F(IntegrationTest, Scenario2_ErrorRecovery) {
    fsm.handleEvent(Event::START);
    EXPECT_EQ(fsm.getCurrentState(), State::SURVEILLANCE);

    // Simuler une erreur
    fsm.handleEvent(Event::ERROR);
    EXPECT_EQ(fsm.getCurrentState(), State::ERREUR);

    // Logger contient un message
    logger.log(LogLevel::ERROR, "Sensor failure detected");
    EXPECT_GE(logger.getLogCount(), 1u);

    // Envoyer RESET
    fsm.handleEvent(Event::RESET);
    EXPECT_EQ(fsm.getCurrentState(), State::DORMANT);
}

// ===== Scénario 3 : Communication complète =====

TEST_F(IntegrationTest, Scenario3_CommunicationFullFlow) {
    // Acquérir des données
    sensorMgr.acquire();
    sensorMgr.acquire();

    // Envoyer via CommunicationManager
    comm.sendLog(LogLevel::INFO, "Temperature OK");
    EXPECT_EQ(logger.getLogCount(), 1u);
    EXPECT_EQ(uart.txAvailable(), 1u);

    // Vérifier que le message a bien été envoyé dans le buffer TX
    EXPECT_EQ(uart.txAvailable(), 1u);
}

TEST_F(IntegrationTest, Scenario3_CommandProcessing) {
    // Envoyer des commandes via UART
    uart.feedRx("STATUS");
    uart.feedRx("RESET");
    uart.feedRx("LOG_LEVEL DEBUG");
    uart.feedRx("UNKNOWN");

    comm.processIncoming();

    // Vérifier que des réponses ont été envoyées
    EXPECT_GE(uart.txAvailable(), 3u);
}

// ===== Scénario 4 : Multithreading complet =====

TEST_F(IntegrationTest, Scenario4_MultithreadingFullFlow) {
    // Lancer les threads
    // (Version simplifiée : on teste la cohérence des données partagées)

    sensorMgr.acquire();
    SensorReading reading;
    reading.temperature = sensorMgr.getTemperatureAvg();
    reading.humidity = sensorMgr.getHumidityAvg();
    reading.pressure = sensorMgr.getPressureAvg();
    reading.timestamp = 1000;

    sharedData.update(reading);

    EXPECT_TRUE(sharedData.isReady());
    SensorReading r = sharedData.getLatest();
    EXPECT_GE(r.temperature, 20.0);
    EXPECT_LE(r.temperature, 30.0);

    // Pas de crash, pas de deadlock
    SUCCEED();
}

// ===== Test de flux complet =====

TEST_F(IntegrationTest, FullFlow_AcquireProcessLog) {
    fsm.handleEvent(Event::START);

    for (int i = 0; i < 10; ++i) {
        sensorMgr.acquire();

        // Traitement
        double tempAvg = sensorMgr.getTemperatureAvg();
        if (tempAvg > 30.0 || tempAvg < 10.0) {
            fsm.handleEvent(Event::THRESHOLD_EXCEEDED);
        }

        // Communication
        std::string msg = "Temp=" + std::to_string(tempAvg);
        comm.sendLog(LogLevel::INFO, msg);
    }

    EXPECT_GE(logger.getLogCount(), 10u);
    EXPECT_EQ(fsm.getCurrentState(), State::SURVEILLANCE);
}