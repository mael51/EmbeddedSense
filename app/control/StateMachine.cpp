#include "app/control/StateMachine.hpp"
#include "app/control/DormantState.hpp"
#include "app/control/SurveillanceState.hpp"
#include "app/control/AlerteState.hpp"
#include "app/control/ErreurState.hpp"

StateMachine::StateMachine()
    : currentState(std::make_unique<DormantState>()) {
}

void StateMachine::handleEvent(Event event) {
    State nextState = currentState->handleEvent(event);
    if (nextState != currentStateEnum) {
        // Transition : créer le nouvel état
        switch (nextState) {
            case State::DORMANT:
                currentState = std::make_unique<DormantState>();
                break;
            case State::SURVEILLANCE:
                currentState = std::make_unique<SurveillanceState>();
                break;
            case State::ALERTE:
                currentState = std::make_unique<AlerteState>();
                break;
            case State::ERREUR:
                currentState = std::make_unique<ErreurState>();
                break;
        }
        currentStateEnum = nextState;
    }
}

std::string StateMachine::getCurrentStateName() const {
    return currentState->getName();
}