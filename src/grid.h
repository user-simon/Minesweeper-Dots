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
	void reposition_mines(cell* from);

	void on_draw(sf::RenderTarget* ctx, UINT game_state, cell* hovered_cell);

	cell* get_cell(VEC2U pos);
};
