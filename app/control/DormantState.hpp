#pragma once
#include "IState.hpp"

class DormantState : public IState {
public:
    State handleEvent(Event event) override;
    std::string getName() const override;
};