#include "app/control/AlerteState.hpp"

State AlerteState::handleEvent(Event event) {
    if (event == Event::RESET) {
        return State::DORMANT;
    }
    if (event == Event::ERROR) {
        return State::ERREUR;
    }
    return State::ALERTE;
}

std::string AlerteState::getName() const {
    return "Alerte";
}