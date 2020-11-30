#include "pch.h"

using namespace design::ui;

ui::ui(VEC2U& client_size)
{
	m_client_size = &client_size;
}

void ui::_draw_text(std::string_view string, VEC2U pos, UINT size, std::string_view font, text_align_t align, sf::RenderTarget* ctx)
{
	sf::Text text = sf::Text(std::string(string), _font(font), size);
	
	switch (align)
	{
	case ALIGN_CENTER:
		pos.x -= text.getLocalBounds().width / 2;
		break;
	case ALIGN_RIGHT:
		pos.x -= text.getLocalBounds().width;
		break;
	}
	text.setPosition(pos);
	ctx->draw(text);
}

void ui::_draw_counter(int num, text_align_t side, sf::RenderTarget* ctx)
{
	std::stringstream stream;
	stream << std::setfill('0');

	if (num < 0)
		stream << "-" << std::setw(2) << -num;
	else
		stream << std::setw(3) << num;
	
	static const sf::Font&		FONT		 = _font("slkscr.ttf");
	static const sf::FloatRect& GLYPH_BOUNDS = FONT.getGlyph('0', TEXT_SIZE, false).bounds;
	static const sf::Texture&	FONT_TEXTURE = FONT.getTexture(TEXT_SIZE);

	static const UINT GLYPH_WIDTH  = GLYPH_BOUNDS.width + 10;
	static const UINT GLYPH_HEIGHT = GLYPH_BOUNDS.height;
	
	VEC2U pos;
	const		 UINT OFFSET_X = m_client_size->x * INSET_K;
	const static UINT OFFSET_Y = PANEL_HEIGHT / 2 - GLYPH_HEIGHT / 2;

	if (side == ALIGN_LEFT)
		pos = VEC2U(OFFSET_X, OFFSET_Y);
	else
		pos = VEC2U(m_client_size->x - OFFSET_X - GLYPH_WIDTH * 3, OFFSET_Y);
	
	// custom font renderer to draw font monospaced
	
	std::string string = stream.str();

	for (UINT i = 0; i < 3; i++)
	{
		const sf::Glyph glyph = FONT.getGlyph(string[i], TEXT_SIZE, false);

		sf::Sprite sprite(FONT_TEXTURE, glyph.textureRect);
		sprite.setPosition(pos);
		
		pos.x += GLYPH_WIDTH;
		
		ctx->draw(sprite);
	}
}

void ui::_draw_rect(VEC2U pos, VEC2U size, sf::Color color, sf::RenderTarget* ctx)
{
	sf::RectangleShape rect(size);
	rect.setPosition(pos);
	rect.setFillColor(color);

	ctx->draw(rect);
}

sf::Font& ui::_font(std::string_view name)
{
	if (!m_fonts.count(name))
	{
		sf::Font font_obj;
		font_obj.loadFromFile(std::string(name));
		m_fonts[name] = font_obj;
	}
	return m_fonts[name];
}

void ui::on_draw(UINT time, int flags, difficulty_t& difficulty, sf::RenderTarget* ctx)
{
	_draw_rect(VEC2U::zero, VEC2U(m_client_size->x, PANEL_HEIGHT), colors::BODY, ctx);
	_draw_counter(flags, ALIGN_LEFT, ctx);
	_draw_counter(time, ALIGN_RIGHT, ctx);
}
