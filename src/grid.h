#pragma once

class grid
{
private:
	difficulty_t* m_difficulty;
	std::vector<std::unique_ptr<cell>> m_cells;

private:
	void _populate();

public:
	void init(difficulty_t*);
	void reset();
	void move_mines(cell* from);

	void on_draw(UINT game_phase, cell* hovered_cell, sf::RenderTarget* ctx);

	cell* get_cell(VEC2U pos);
};
