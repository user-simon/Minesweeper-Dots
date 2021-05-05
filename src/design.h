#pragma once
#include <SFML/Graphics/Color.hpp>
#include "math_vector/math_vector.h"

#define COLOR(r,g,b) sf::Color(r * 255, g * 255, b * 255)

namespace design
{
    namespace cells
    {
        constexpr uint  SIZE     = 45;
        constexpr float DOT_SIZE = SIZE * 0.4f;

        namespace colors
        {
            const static sf::Color NUMBER[] =
            {
                COLOR(0,   0,   1.0),	// blue
                COLOR(0,   0.5, 0),		// dark green
                COLOR(1.0, 0,   0),		// red
                COLOR(0,   0,   0.5),	// dark blue
                COLOR(0.5, 0,   0),		// dark red
                COLOR(0,   0.5, 0.5),	// dark cyan
                COLOR(0,   0,   0),		// black
                COLOR(0.5, 0.5, 0.5),	// gray
            };
            const static sf::Color MINE = COLOR(1.0, 1.0, 1.0); // white
            const static sf::Color FLAG = COLOR(1.0, 0.5, 0.0); // orange
            
            const static sf::Color FILLED  = COLOR(0.12, 0.12, 0.2);
            const static sf::Color OPENED  = COLOR(0.07, 0.07, 0.17);
            const static sf::Color HOVERED = COLOR(0.15, 0.15, 0.25);
        }
    }

    namespace ui
    {
        constexpr char FONT_PATH[] = "slkscr.ttf";

        constexpr uint  PANEL_HEIGHT    = cells::SIZE * 2;
        constexpr uint  COUNTER_SIZE    = PANEL_HEIGHT * 0.6;
        constexpr float COUNTER_INSET_K = 0.08f;
        
        constexpr uint  SCORE_SIZE = cells::SIZE * 2.5;
        constexpr uint  TITLE_SIZE = SCORE_SIZE * 0.6;

        namespace colors
        {
            const static sf::Color BODY = COLOR(0.1, 0.1, 0.15);
            const static sf::Color TEXT = COLOR(1.0, 1.0, 1.0);
        }
    }
}
