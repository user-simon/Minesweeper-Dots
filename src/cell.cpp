#include "pch.h"
#include "cell.h"

cell::cell(const uint2d& pos) : pos(pos)
{
    neighbours.reserve(8); // most cells have 8 neighbours, shrink_to_fit called later
    init();
}

void cell::init()
{
    number   = 0;
    is_open  = 0;
    has_flag = 0;
    has_mine = 0;
}

int cell::open()
{
    if (is_open || has_flag)
        return 0;

    if (has_mine)
        return -1;
    
    is_open = true;
    
    uint opened_cells = 1;

    // recursively open all connecting cells with no surrounding mines
    if (!number)
    {
        for (cell* n : neighbours)
            opened_cells += n->open();
    }
    return opened_cells;
}

int cell::open_neighbours()
{
    if (!is_open)
        return 0;

    uint flags = 0;

    // count all surrounding flags
    for (cell* nc : neighbours)
    {
        if (nc->has_flag)
            flags++;
    }

    // ignore if there aren't as many surrounding
    // mines as flags
    if (flags != number)
        return 0;

    // open neighbouring cells

    uint opened_cells = 0;

    for (cell* nc : neighbours)
    {
        if (int buf; (buf = nc->open()) >= 0)
            opened_cells += buf;
        else
            return -1;
    }
    return opened_cells;
}

void cell::on_draw(PHASE game_phase, bool hovered, sf::RenderTarget& ctx) const
{
    using namespace design::cells;

    static sf::RectangleShape body({ SIZE, SIZE });
    static sf::RectangleShape dot({ DOT_SIZE, DOT_SIZE });

    constexpr static uint2d BODY_OFFSET = uint2d(0, design::ui::PANEL_HEIGHT);

    constexpr static uint   DOT_INSET_DIST = (SIZE - DOT_SIZE) / 2;
    constexpr static uint2d DOT_OFFSET     = uint2d(DOT_INSET_DIST, DOT_INSET_DIST + design::ui::PANEL_HEIGHT);

    // body
    {
        uint2d body_pos = pos * SIZE + BODY_OFFSET;

        body.setPosition(body_pos);
        
        if (is_open)
            body.setFillColor(colors::OPENED);
        else if (hovered)
            body.setFillColor(colors::HOVERED);
        else
            body.setFillColor(colors::FILLED);

        ctx.draw(body);
    }
    
    // dot
    if ((is_open && number) || has_flag || (has_mine && game_phase > PHASE::STARTED))
    {
        uint2d dot_pos = pos * SIZE + DOT_OFFSET;

        dot.setPosition(dot_pos);

        if (is_open)
            dot.setFillColor(colors::NUMBER[number - 1]);
        else if (has_flag || has_mine && game_phase == PHASE::WON)
            dot.setFillColor(colors::FLAG);
        else
            dot.setFillColor(colors::MINE);
        
        ctx.draw(dot);
    }
}
