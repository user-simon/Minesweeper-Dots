#pragma once
#include <SFML/System/Clock.hpp>

#include "vector_n/vector_n.h"

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
