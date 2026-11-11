#include "app/control/DormantState.hpp"

State DormantState::handleEvent(Event event) {
    if (event == Event::START) {
        return State::SURVEILLANCE;
    }
    return State::DORMANT;
}

std::string DormantState::getName() const {
    return "Dormant";
}