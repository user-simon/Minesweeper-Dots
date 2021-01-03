#include "pch.h"
#include "cell.h"

cell::cell(vec2u pos)
{
	m_pos = pos;
	m_neighbours.reserve(3);

	init();
}

void cell::init()
{
	m_number = 0;
	m_data = DATA_NONE;
}

int cell::open()
{
	if (has(DATA_OPEN) || has(DATA_FLAG))
		return 0;

	if (has(DATA_MINE))
		return -1;
	
	set(DATA_OPEN, true);
	
	uint opened_cells = 1;

	// recursively open all connecting cells with no surrounding mines
	if (!m_number)
	{
		for (cell* n : m_neighbours)
			opened_cells += n->open();
	}
	return opened_cells;
}

int cell::open_neighbours()
{
	if (!has(DATA_OPEN))
		return 0;

	uint flags = 0;

	for (cell* nc : m_neighbours)
	{
		if (nc->has(DATA_FLAG))
			flags++;
	}

	if (flags != m_number)
		return 0;

	uint opened_cells = 0;

	for (cell* nc : m_neighbours)
	{
		if (int buf; (buf = nc->open()) >= 0)
			opened_cells += buf;
		else
			return -1;
	}
	return opened_cells;
}

void cell::on_draw(uint game_phase, bool hovered, sf::RenderTarget* ctx)
{
	using namespace design::cells;

	static sf::RectangleShape body(vec2u(SIZE, SIZE));
	static sf::RectangleShape dot(vec2u(DOT_SIZE, DOT_SIZE));

	constexpr vec2u BODY_OFFSET = vec2u(0, design::ui::PANEL_HEIGHT);

	constexpr uint  DOT_INSET_DIST = (SIZE - DOT_SIZE) / 2;
	constexpr vec2u DOT_OFFSET     = vec2u(DOT_INSET_DIST, DOT_INSET_DIST + design::ui::PANEL_HEIGHT);
	
	// body
	{
		body.setPosition(m_pos * SIZE + BODY_OFFSET);
	
		if (has(DATA_OPEN))
			body.setFillColor(colors::OPENED);
		else if (hovered)
			body.setFillColor(colors::HOVERED);
		else
			body.setFillColor(colors::FILLED);

		ctx->draw(body);
	}
	
	// dot
	if ((has(DATA_OPEN) && m_number) || has(DATA_FLAG) || (has(DATA_MINE) && game_phase > GAME_STARTED))
	{
		dot.setPosition(m_pos * SIZE + DOT_OFFSET);

		if (has(DATA_OPEN))
			dot.setFillColor(colors::NUMBER[m_number - 1]);
		else if (has(DATA_FLAG) || has(DATA_MINE) && game_phase == GAME_WON)
			dot.setFillColor(colors::FLAG);
		else
			dot.setFillColor(colors::MINE);
		
		ctx->draw(dot);
	}
}

bool cell::has(data_t flag) const
{
	return m_data & flag;
}

void cell::set(data_t flag, bool val)
{
	if (has(flag) == val)
		return;

	if (val)
		m_data |= flag;
	else
		m_data &= ~flag;

	// update neighbours' numbers if we placed or removed a mine
	if (flag == DATA_MINE)
	{
		const uint delta = val ? 1 : -1;

		for (cell* c : m_neighbours)
			c->number() += delta;
	}
}

bool cell::toggle(data_t flag)
{
	m_data ^= flag;
	return has(flag);
}

vec2u& cell::pos()
{
	return m_pos;
}

uint& cell::number()
{
	return m_number;
}

void cell::add_neighbour(cell* c)
{
	m_neighbours.push_back(c);
}

std::vector<cell*>& cell::neighbours()
{
	return m_neighbours;
}
