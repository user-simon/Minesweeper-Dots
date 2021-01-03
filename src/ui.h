#pragma once

enum text_align_t
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

class ui
{
private:
	vec2u* m_client_size;
	std::unordered_map<std::string_view, sf::Font> m_fonts;

	std::string _format_counter(float num, int precision = 0);

	void _draw_text(std::string_view string, vec2i pos, uint size, std::string_view font, text_align_t align, sf::RenderTarget* ctx);
	void _draw_rect(vec2u pos, vec2u size, sf::Color color, sf::RenderTarget* ctx);
	void _draw_counter(int num, text_align_t side, sf::RenderTarget* ctx);

	sf::Font& _font(std::string_view name);

public:
	ui(vec2u& client_size);

	void on_draw(game_state_t game_state, sf::RenderTarget* ctx);
};
