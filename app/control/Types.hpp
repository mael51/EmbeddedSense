#pragma once
#include <cstdint>

enum class State : uint8_t {
    DORMANT,
    SURVEILLANCE,
    ALERTE,
    ERREUR
};

enum class Event : uint8_t {
    START,
    TIMEOUT,
    THRESHOLD_EXCEEDED,
    ERROR,
    RESET
};