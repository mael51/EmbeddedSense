#pragma once
#include "Types.hpp"
#include "IState.hpp"
#include <memory>
#include <string>

class StateMachine {
private:
    std::unique_ptr<IState> currentState;
    State currentStateEnum = State::DORMANT;

public:
    StateMachine();
    void handleEvent(Event event);
    State getCurrentState() const { return currentStateEnum; }
    std::string getCurrentStateName() const;
};