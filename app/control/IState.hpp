#pragma once
#include "Types.hpp"
#include <string>

class IState {
public:
    virtual ~IState() = default;
    virtual State handleEvent(Event event) = 0;
    virtual std::string getName() const = 0;
};