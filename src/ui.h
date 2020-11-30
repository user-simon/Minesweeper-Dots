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
	VEC2U* m_client_size;
	std::unordered_map<std::string_view, sf::Font> m_fonts;

	void _draw_text(std::string_view string, VEC2U pos, UINT size, std::string_view font, text_align_t align, sf::RenderTarget* ctx);
	void _draw_counter(int num, text_align_t side, sf::RenderTarget* ctx);
	void _draw_rect(VEC2U pos, VEC2U size, sf::Color color, sf::RenderTarget* ctx);

	sf::Font& _font(std::string_view name);

public:
	ui(VEC2U& client_size);

	void on_draw(UINT time, int flags, difficulty_t& difficulty, sf::RenderTarget* ctx);
};
