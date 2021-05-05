#pragma once
#include <SFML/System/Clock.hpp>

#include "math_vector/math_vector.h"

enum class PHASE
{
    READY,
    STARTED,
    WON,
    LOST,
};

struct game_state
{
    PHASE phase;
    float duration;

    // started
    uint cells_left;
    int flags_left;
    sf::Clock clock;

    // won
    bool beat_pb;
    bool beat_avg;
};
