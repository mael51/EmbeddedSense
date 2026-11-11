#include "app/control/SurveillanceState.hpp"

State SurveillanceState::handleEvent(Event event) {
    switch (event) {
        case Event::THRESHOLD_EXCEEDED:
            return State::ALERTE;
        case Event::ERROR:
            return State::ERREUR;
        default:
            return State::SURVEILLANCE;
    }
}

std::string SurveillanceState::getName() const {
    return "Surveillance";
}