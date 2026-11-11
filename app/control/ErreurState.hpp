#pragma once
#include "IState.hpp"

class ErreurState : public IState {
public:
    State handleEvent(Event event) override;
    std::string getName() const override;
};