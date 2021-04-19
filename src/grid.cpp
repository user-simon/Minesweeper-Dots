#include "pch.h"
#include "grid.h"

void grid::init(uint2d size)
{
    m_size = size;

    // create cells

    m_cells.clear();
    m_cells.reserve(size.x * size.y);

    for (uint y = 0; y < size.y; y++)
    {
        for (uint x = 0; x < size.x; x++)
        {
            m_cells.push_back(std::make_unique<cell>(uint2d(x, y)));
        }
    }
    
    // set neighbours

    for (std::unique_ptr<cell>& c : m_cells)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                const int2d dpos = int2d(dx, dy);

                if (!dpos)
                    continue;

                const uint2d npos = c->pos + dpos;

                if (npos.x >= m_size.x || npos.y >= m_size.y)
                    continue;

                c->neighbours.push_back(get_cell(npos));
            }
        }
        // some cells will have over-allocated memory for neighbours
        c->neighbours.shrink_to_fit();
    }
}

void grid::reset()
{
    for (std::unique_ptr<cell>& c : m_cells)
        c->init();
}

void grid::populate(uint mines, uint2d around)
{
    for (uint i = 0; i < mines; i++)
    {
        uint2d cand_pos = uint2d::ZERO;
        cell* cand_cell = nullptr;

        do
        {
            cand_pos  = get_random_pos(m_size);
            cand_cell = get_cell(cand_pos);
        }
        while (cand_cell->has_mine || (cand_pos.distance2(around)) <= 2);
        
        // place the mine

        cand_cell->has_mine = true;

        for (cell* neighbour : cand_cell->neighbours)
        {
            neighbour->number++;
        }
    }
}

void grid::on_draw(PHASE game_phase, cell* hovered_cell, sf::RenderTarget& ctx)
{
    for (std::unique_ptr<cell>& c : m_cells)
        c->on_draw(game_phase, c.get() == hovered_cell, ctx);
}

cell* grid::get_cell(uint2d pos)
{
    return m_cells[pos.x + m_size.x * pos.y].get();
}
