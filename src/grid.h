#pragma once
#include <vector>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include "vector_n/vector_n.h"

#include "cell.h"
#include "game_state.h"

class grid
{
private:
    uint2d m_size;
    std::vector<std::unique_ptr<cell>> m_cells;

public:
    void init(uint2d size);
    void reset();
    void populate(uint mines, uint2d around);
    void on_draw(PHASE game_phase, cell* hovered_cell, sf::RenderTarget& ctx);
    cell* get_cell(uint2d pos);
};
