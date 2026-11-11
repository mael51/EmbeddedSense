#include "app/control/ErreurState.hpp"

State ErreurState::handleEvent(Event event) {
    if (event == Event::RESET) {
        return State::DORMANT;
    }
    return State::ERREUR;
}

std::string ErreurState::getName() const {
    return "Erreur";
}