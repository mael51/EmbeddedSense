#pragma once
#include "IState.hpp"

class SurveillanceState : public IState {
public:
    State handleEvent(Event event) override;
    std::string getName() const override;
};