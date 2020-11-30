#include "pch.h"

void grid::init(difficulty_t* difficulty)
{
	m_difficulty = difficulty;

	// create cells

	m_cells.clear();
	m_cells.reserve(m_difficulty->cell_count);

	for (UINT i = 0; i < m_difficulty->cell_count; i++)
  		m_cells.emplace_back(std::make_unique<cell>(VEC2U(i % m_difficulty->size.x, i / m_difficulty->size.x)));
	
	// get neighbours

	for (std::unique_ptr<cell>& c : m_cells)
	{
		for (int dx = -1; dx <= 1; dx++)
		{
			for (int dy = -1; dy <= 1; dy++)
			{
				VEC2U dpos = VEC2U(dx, dy);

				if (!dpos)
					continue;

				VEC2U npos = c->pos() + dpos;

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

void grid::reposition_mines(cell* clicked)
{
	UINT mines_to_move = clicked->number();
	
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

	for (UINT i = 0; i < mines_to_move; i++)
	{
		cell* new_mine_cell = nullptr;
		VEC2U mine_pos;
		VEC2I delta;

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

void grid::on_draw(sf::RenderTarget* ctx, UINT game_state, cell* hovered_cell)
{
	for (std::unique_ptr<cell>& c : m_cells)
		c->on_draw(ctx, game_state, c.get() == hovered_cell);
}

cell* grid::get_cell(VEC2U pos)
{
	return m_cells[pos.x + m_difficulty->size.x * pos.y].get();
}

void grid::_populate()
{
	// place mines

	for (UINT i = 0; i < m_difficulty->mines; i++)
	{
		UINT pos;

		do
		{
			pos = get_random(m_difficulty->cell_count);
		} while (m_cells[pos]->has(cell::DATA_MINE));

		m_cells[pos]->set(cell::DATA_MINE, true);
	}
}
