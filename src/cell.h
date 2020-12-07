#pragma once

struct cell
{
public:
	enum data_t : UINT
	{
		DATA_NONE = 0,
		DATA_OPEN = 0b1 << 0,
		DATA_MINE = 0b1 << 1,
		DATA_FLAG = 0b1 << 2,
	};
	
private:
	VEC2U m_pos;
	std::vector<cell*> m_neighbours;

	UINT m_number;
	UINT m_data;

public:
	cell(VEC2U pos);

	void init();
	int open();
	int open_neighbours();

	void on_draw(UINT game_phase, bool hovered, sf::RenderTarget* ctx);

	bool has(data_t flag) const;
	void set(data_t flag, bool val);
	bool toggle(data_t flag);

	VEC2U& pos();
	UINT& number();

	void add_neighbour(cell*);
	std::vector<cell*>& neighbours();
};
