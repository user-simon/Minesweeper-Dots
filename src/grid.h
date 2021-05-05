#pragma once
#include <vector>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include "math_vector/math_vector.h"

#include "cell.h"
#include "game_state.h"

class grid
{
private:
    uint2d m_size;
    std::vector<cell> m_cells;

public:
    void init(const uint2d& size);
    void reset();
    void populate(uint mines, const uint2d& around);
    void on_draw(PHASE game_phase, cell* hovered_cell, sf::RenderTarget& ctx) const;
    cell* get_cell(const uint2d& pos);
};
