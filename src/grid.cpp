#include "pch.h"

void grid::init(difficulty_t* difficulty)
{
	m_difficulty = difficulty;

	// create cells

	m_cells.clear();
	m_cells.reserve(m_difficulty->cell_count);

	for (uint i = 0; i < m_difficulty->cell_count; i++)
  		m_cells.emplace_back(std::make_unique<cell>(vec2u(i % m_difficulty->size.x, i / m_difficulty->size.x)));
	
	// set neighbours

	for (std::unique_ptr<cell>& c : m_cells)
	{
		for (int dx = -1; dx <= 1; dx++)
		{
			for (int dy = -1; dy <= 1; dy++)
			{
				vec2u dpos = vec2u(dx, dy);

				if (!dpos)
					continue;

				vec2u npos = c->pos() + dpos;

				if (npos.x >= m_difficulty->size.x || npos.y >= m_difficulty->size.y)
					continue;

				c->add_neighbour(get_cell(npos));
			}
		}
	}
}

void grid::reset()
{
	for (std::unique_ptr<cell>& c : m_cells)
		c->init();

	_populate();
}

void grid::move_mines(cell* clicked)
{
	uint mines_to_move = clicked->number();
	
	if (clicked->has(cell::DATA_MINE))
	{
		clicked->set(cell::DATA_MINE, false);
		mines_to_move++;
	}

	if (!mines_to_move)
		return;

	for (cell* nc : clicked->neighbours())
		nc->set(cell::DATA_MINE, false);

	// re-add the mines

	for (uint i = 0; i < mines_to_move; i++)
	{
		cell* new_mine_cell = nullptr;
		vec2u mine_pos;
		vec2i delta;

		// randomize mine position until a cell outside the 3x3 grid around the clicked cell
		// and which does not have a mine is found
		do
		{
			mine_pos.x = get_random(m_difficulty->size.x);
			mine_pos.y = get_random(m_difficulty->size.y);
			
			delta = (clicked->pos() - mine_pos).abs();
			new_mine_cell = get_cell(mine_pos);
		}
		while (new_mine_cell->has(cell::DATA_MINE) || (delta.x <= 1 && delta.y <= 1));
		
		new_mine_cell->set(cell::DATA_MINE, true);
	}
}

void grid::on_draw(uint game_phase, cell* hovered_cell, sf::RenderTarget* ctx)
{
	for (std::unique_ptr<cell>& c : m_cells)
		c->on_draw(game_phase, c.get() == hovered_cell, ctx);
}

cell* grid::get_cell(vec2u pos)
{
	return m_cells[pos.x + m_difficulty->size.x * pos.y].get();
}

void grid::_populate()
{
	// place mines

	for (uint i = 0; i < m_difficulty->mines; i++)
	{
		uint pos;

		do
		{
			pos = get_random(m_difficulty->cell_count);
		}
		while (m_cells[pos]->has(cell::DATA_MINE));

		m_cells[pos]->set(cell::DATA_MINE, true);
	}
}
