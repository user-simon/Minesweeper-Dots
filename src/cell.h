#pragma once
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>
#include "vector_n/vector_n.h"

struct cell
{
    bool is_open  : 1;
    bool has_mine : 1;
    bool has_flag : 1;

    uint2d pos;
    std::vector<cell*> neighbours;

    uint number;

    cell(const uint2d& pos);

    // initializes/resets variables
    void init();

    // opens cell and all surrounding cells with
    // no neighbouring mines. returns the number
    // of opened cells or -1 if a cell containing
    // a mine was opened
    int open();

    // opens all surrounding cells if there are
    // as many neighbouring flags as mines. see
    // cell::open for return value
    int open_neighbours();

    void on_draw(PHASE game_phase, bool hovered, sf::RenderTarget& ctx);
};
