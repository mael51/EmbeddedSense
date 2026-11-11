#include <gtest/gtest.h>
#include "app/control/Types.hpp"
#include "app/control/StateMachine.hpp"

using namespace std;

TEST(StateMachineTest, InitialState) {
    StateMachine fsm;
    EXPECT_EQ(fsm.getCurrentState(), State::DORMANT);
    EXPECT_EQ(fsm.getCurrentStateName(), "Dormant");
}

TEST(StateMachineTest, DormantToSurveillance) {
    StateMachine fsm;
    fsm.handleEvent(Event::START);
    EXPECT_EQ(fsm.getCurrentState(), State::SURVEILLANCE);
}

TEST(StateMachineTest, SurveillanceToAlerte) {
    StateMachine fsm;
    fsm.handleEvent(Event::START);
    fsm.handleEvent(Event::THRESHOLD_EXCEEDED);
    EXPECT_EQ(fsm.getCurrentState(), State::ALERTE);
}

TEST(StateMachineTest, SurveillanceToErreur) {
    StateMachine fsm;
    fsm.handleEvent(Event::START);
    fsm.handleEvent(Event::ERROR);
    EXPECT_EQ(fsm.getCurrentState(), State::ERREUR);
}

TEST(StateMachineTest, AlerteToDormant) {
    StateMachine fsm;
    fsm.handleEvent(Event::START);
    fsm.handleEvent(Event::THRESHOLD_EXCEEDED);
    fsm.handleEvent(Event::RESET);
    EXPECT_EQ(fsm.getCurrentState(), State::DORMANT);
}

TEST(StateMachineTest, AlerteToErreur) {
    StateMachine fsm;
    fsm.handleEvent(Event::START);
    fsm.handleEvent(Event::THRESHOLD_EXCEEDED);
    fsm.handleEvent(Event::ERROR);
    EXPECT_EQ(fsm.getCurrentState(), State::ERREUR);
}

TEST(StateMachineTest, ErreurToDormant) {
    StateMachine fsm;
    fsm.handleEvent(Event::START);
    fsm.handleEvent(Event::ERROR);
    fsm.handleEvent(Event::RESET);
    EXPECT_EQ(fsm.getCurrentState(), State::DORMANT);
}

TEST(StateMachineTest, InvalidTransitionIgnored) {
    StateMachine fsm;
    // Depuis DORMANT, un événement invalide ne change rien
    fsm.handleEvent(Event::TIMEOUT);
    EXPECT_EQ(fsm.getCurrentState(), State::DORMANT);
    fsm.handleEvent(Event::RESET);
    EXPECT_EQ(fsm.getCurrentState(), State::DORMANT);
}

TEST(StateMachineTest, FullCycle) {
    StateMachine fsm;
    EXPECT_EQ(fsm.getCurrentState(), State::DORMANT);

    fsm.handleEvent(Event::START);
    EXPECT_EQ(fsm.getCurrentState(), State::SURVEILLANCE);

    fsm.handleEvent(Event::THRESHOLD_EXCEEDED);
    EXPECT_EQ(fsm.getCurrentState(), State::ALERTE);

    fsm.handleEvent(Event::RESET);
    EXPECT_EQ(fsm.getCurrentState(), State::DORMANT);
}

TEST(StateMachineTest, SurveillanceStaysOnTimeout) {
    StateMachine fsm;
    fsm.handleEvent(Event::START);
    fsm.handleEvent(Event::TIMEOUT);
    EXPECT_EQ(fsm.getCurrentState(), State::SURVEILLANCE);
}

TEST(StateMachineTest, ErrorFromAnyState) {
    // Depuis SURVEILLANCE
    {
        StateMachine fsm;
        fsm.handleEvent(Event::START);
        fsm.handleEvent(Event::ERROR);
        EXPECT_EQ(fsm.getCurrentState(), State::ERREUR);
    }
    // Depuis ALERTE
    {
        StateMachine fsm;
        fsm.handleEvent(Event::START);
        fsm.handleEvent(Event::THRESHOLD_EXCEEDED);
        fsm.handleEvent(Event::ERROR);
        EXPECT_EQ(fsm.getCurrentState(), State::ERREUR);
    }
}