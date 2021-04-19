#pragma once
#include <fstream>
#include <iomanip>
#include <sstream>
#include <random>
#include <chrono>

#include <SFML/Graphics.hpp>

#include "vector_n/vector_n.h"
#include "json/include/nlohmann/json.hpp"

#include "design.h"
#include "game_state.h"
#include "difficulty.h"

static uint get_random(uint max)
{
    static std::random_device rd;
    static std::mt19937 gen = std::mt19937(rd());
    static std::uniform_int_distribution<uint> dist;

    return dist(gen) % max;
}

inline uint2d get_random_pos(uint2d max)
{
    return { get_random(max.x), get_random(max.y) };
}
