#include <gtest/gtest.h>
#include "services/logger/LogLevel.hpp"
#include "services/logger/Logger.hpp"
#include "app/communication/MockUART.hpp"
#include "app/communication/CommunicationManager.hpp"

// ===== Tests Logger =====

TEST(LoggerTest, LogStoresMessage) {
    Logger logger(LogLevel::DEBUG);
    logger.log(LogLevel::INFO, "Test message");
    EXPECT_EQ(logger.getLogCount(), 1u);
    EXPECT_NE(logger.getLogs()[0].find("Test message"), std::string::npos);
}

TEST(LoggerTest, MinLevelFilters) {
    Logger logger(LogLevel::WARN);
    logger.log(LogLevel::DEBUG, "Debug msg");
    logger.log(LogLevel::INFO, "Info msg");
    logger.log(LogLevel::WARN, "Warn msg");
    logger.log(LogLevel::ERROR, "Error msg");
    EXPECT_EQ(logger.getLogCount(), 2u);  // Seulement WARN et ERROR
}

TEST(LoggerTest, FormatContainsLevel) {
    Logger logger(LogLevel::DEBUG);
    std::string formatted = logger.format(LogLevel::ERROR, "Something broke");
    EXPECT_NE(formatted.find("[ERROR]"), std::string::npos);
    EXPECT_NE(formatted.find("Something broke"), std::string::npos);
}

TEST(LoggerTest, FormatContainsTimestamp) {
    Logger logger(LogLevel::DEBUG);
    std::string formatted = logger.format(LogLevel::INFO, "Hello");
    // Vérifie la présence de crochets de date
    EXPECT_NE(formatted.find("["), std::string::npos);
    EXPECT_NE(formatted.find("]"), std::string::npos);
}

TEST(LoggerTest, ClearLogs) {
    Logger logger(LogLevel::DEBUG);
    logger.log(LogLevel::INFO, "Msg 1");
    logger.log(LogLevel::INFO, "Msg 2");
    EXPECT_EQ(logger.getLogCount(), 2u);
    logger.clear();
    EXPECT_EQ(logger.getLogCount(), 0u);
}

TEST(LoggerTest, GetMinLevel) {
    Logger logger(LogLevel::WARN);
    EXPECT_EQ(logger.getMinLevel(), LogLevel::WARN);
}

// ===== Tests MockUART =====

TEST(MockUARTTest, SendAndReceive) {
    MockUART uart;
    uart.feedRx("Hello");
    std::string received = uart.receive();
    EXPECT_EQ(received, "Hello");
}

TEST(MockUARTTest, TxAvailable) {
    MockUART uart;
    uart.send("Data1");
    uart.send("Data2");
    EXPECT_EQ(uart.txAvailable(), 2u);
}

TEST(MockUARTTest, RxAvailable) {
    MockUART uart;
    uart.feedRx("Data1");
    EXPECT_EQ(uart.rxAvailable(), 1u);
    uart.receive();
    EXPECT_EQ(uart.rxAvailable(), 0u);
}

TEST(MockUARTTest, ReceiveEmpty) {
    MockUART uart;
    std::string received = uart.receive();
    EXPECT_EQ(received, "");
}

// ===== Tests CommunicationManager =====

TEST(CommManagerTest, SendLog) {
    MockUART uart;
    Logger logger(LogLevel::DEBUG);
    CommunicationManager comm(uart, logger);

    comm.sendLog(LogLevel::INFO, "Temperature OK");

    EXPECT_EQ(logger.getLogCount(), 1u);
    EXPECT_EQ(uart.txAvailable(), 1u);
}

TEST(CommManagerTest, ProcessIncomingStatus) {
    MockUART uart;
    Logger logger(LogLevel::DEBUG);
    CommunicationManager comm(uart, logger);

    uart.feedRx("STATUS");
    comm.processIncoming();

    // La réponse doit être dans txBuffer
    EXPECT_GE(uart.txAvailable(), 1u);
}

TEST(CommManagerTest, ProcessIncomingReset) {
    MockUART uart;
    Logger logger(LogLevel::DEBUG);
    CommunicationManager comm(uart, logger);

    uart.feedRx("RESET");
    comm.processIncoming();

    EXPECT_GE(uart.txAvailable(), 1u);
}

TEST(CommManagerTest, ProcessIncomingUnknown) {
    MockUART uart;
    Logger logger(LogLevel::DEBUG);
    CommunicationManager comm(uart, logger);

    uart.feedRx("UNKNOWN_CMD");
    comm.processIncoming();

    EXPECT_GE(uart.txAvailable(), 1u);
}

TEST(CommManagerTest, HandleCommand) {
    MockUART uart;
    Logger logger(LogLevel::DEBUG);
    CommunicationManager comm(uart, logger);

    EXPECT_EQ(comm.handleCommand("STATUS"), "OK:STATUS");
    EXPECT_EQ(comm.handleCommand("RESET"), "OK:RESET");
    EXPECT_EQ(comm.handleCommand("LOG_LEVEL DEBUG"), "OK:LOG_LEVEL_CHANGED");
    EXPECT_EQ(comm.handleCommand("FOO"), "ERR:UNKNOWN_CMD");
}