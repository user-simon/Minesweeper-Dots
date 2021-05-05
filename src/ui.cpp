#include "pch.h"
#include "ui.h"

using namespace design::ui;

ui::ui(const uint2d& client_size) : m_client_size(client_size)
{
    
}

std::string ui::_format_counter(float num, uint precision)
{
    std::stringstream stream;
    stream << std::setfill('0') << std::fixed << std::setprecision(precision);

    if (num < 0)
        stream << "-" << std::setw(2 + precision) << -num;
    else
        stream << std::setw(3 + precision) << num;

    return stream.str();
}

void ui::_draw_text(const std::string& string, uint2d pos, uint size, const std::string& font, TEXT_ALIGN align, sf::RenderTarget& ctx)
{
    sf::Text text = sf::Text(std::string(string), _font(font), size);
    text.setFillColor(colors::TEXT);

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
    ctx.draw(text);
}

void ui::_draw_rect(uint2d pos, uint2d size, sf::Color color, sf::RenderTarget& ctx)
{
    sf::RectangleShape rect(size);
    rect.setPosition(pos);
    rect.setFillColor(color);

    ctx.draw(rect);
}

void ui::_draw_counter(int num, TEXT_ALIGN side, sf::RenderTarget& ctx)
{
    static const sf::Font&      FONT         = _font(FONT_PATH);
    static const sf::FloatRect& GLYPH_BOUNDS = FONT.getGlyph('0', COUNTER_SIZE, false).bounds;
    static const sf::Texture&   FONT_TEXTURE = FONT.getTexture(COUNTER_SIZE);

    static const uint GLYPH_WIDTH  = GLYPH_BOUNDS.width + 10;
    static const uint GLYPH_HEIGHT = GLYPH_BOUNDS.height;
    
    const        uint OFFSET_X = m_client_size.x * COUNTER_INSET_K;
    const static uint OFFSET_Y = PANEL_HEIGHT / 2 - GLYPH_HEIGHT / 2;

    uint2d pos;

    if (side == ALIGN_LEFT)
        pos = uint2d(OFFSET_X, OFFSET_Y);
    else
        pos = uint2d(m_client_size.x - OFFSET_X - GLYPH_WIDTH * 3, OFFSET_Y);
    
    // custom font renderer to draw font monospaced

    std::string string = _format_counter(num);

    for (uint i = 0; i < 3; i++)
    {
        const sf::Glyph glyph = FONT.getGlyph(string[i], COUNTER_SIZE, false);

        sf::Sprite sprite(FONT_TEXTURE, glyph.textureRect);
        sprite.setPosition(pos);
        
        pos.x += GLYPH_WIDTH;
        
        ctx.draw(sprite);
    }
}

sf::Font& ui::_font(const std::string& name)
{
    if (!m_fonts.count(name))
    {
        sf::Font font_obj;
        font_obj.loadFromFile(name);
        return m_fonts[name] = font_obj;
    }
    return m_fonts[name];
}

void ui::on_draw(const game_state& game_state, sf::RenderTarget& ctx)
{
    _draw_rect(uint2d::zero, uint2d(m_client_size.x, PANEL_HEIGHT), colors::BODY, ctx);
    _draw_counter(game_state.flags_left, ALIGN_LEFT, ctx);
    _draw_counter(game_state.duration, ALIGN_RIGHT, ctx);

    if (game_state.phase == PHASE::WON)
    {
        sf::Color color = colors::BODY;
        color.a = 128;
        _draw_rect(uint2d::zero, m_client_size, color, ctx);

        uint2d time_pos = m_client_size * 0.5 - uint2d(0, SCORE_SIZE / 2);

        if (game_state.beat_pb)
        {
            uint2d title_pos = time_pos;

            title_pos.y *= 0.85f;
            time_pos.y  *= 1.15f;

            _draw_text("NEW PB!", title_pos, TITLE_SIZE, FONT_PATH, ALIGN_CENTER, ctx);
        }
        _draw_text(_format_counter(game_state.duration, 2), time_pos, SCORE_SIZE, FONT_PATH, ALIGN_CENTER, ctx);
    }
}
