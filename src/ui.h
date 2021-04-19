#pragma once
#include <unordered_map>
#include <string_view>

#include <SFML/Graphics.hpp>
#include "vector_n/vector_n.h"

#include "game_state.h"

enum TEXT_ALIGN
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

class ui
{
private:
    uint2d& m_client_size;
	std::unordered_map<std::string_view, sf::Font> m_fonts;

private:
	std::string _format_counter(float num, uint precision = 0);

	void _draw_text(const std::string& string, uint2d pos, uint size, const std::string& font, TEXT_ALIGN align, sf::RenderTarget& ctx);
	void _draw_rect(uint2d pos, uint2d size, sf::Color color, sf::RenderTarget& ctx);
	void _draw_counter(int num, TEXT_ALIGN side, sf::RenderTarget& ctx);

	sf::Font& _font(const std::string& name);

public:
	ui(uint2d& client_size);

	void on_draw(const game_state& game_state, sf::RenderTarget& ctx);
};
