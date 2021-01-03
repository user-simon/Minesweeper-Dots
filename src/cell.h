#pragma once

struct cell
{
public:
	enum data_t : uint
	{
		DATA_NONE = 0,
		DATA_OPEN = 0b1 << 0,
		DATA_MINE = 0b1 << 1,
		DATA_FLAG = 0b1 << 2,
	};
	
private:
	vec2u m_pos;
	std::vector<cell*> m_neighbours;

	uint m_number;
	uint m_data;

public:
	cell(vec2u pos);

	void init();
	int open();
	int open_neighbours();

	void on_draw(uint game_phase, bool hovered, sf::RenderTarget* ctx);

	bool has(data_t flag) const;
	void set(data_t flag, bool val);
	bool toggle(data_t flag);

	vec2u& pos();
	uint& number();

	void add_neighbour(cell*);
	std::vector<cell*>& neighbours();
};
