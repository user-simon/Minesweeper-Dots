#pragma once
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>
#include "math_vector/math_vector.h"

#include "sfml_app.h"
#include "grid.h"
#include "ui.h"
#include "difficulty.h"
#include "game_state.h"

class minesweeper_app : public sfml_app
{
private:
    grid m_grid;
    ui m_ui;

    std::vector<difficulty> m_difficulties;
    difficulty* m_difficulty;

    game_state m_state;
    cell* m_hovered_cell;

private:
    void _start();
    void _end();
    void _reset();

    void on_draw(sf::RenderTarget& ctx)   override;
    void on_key_down(uint key)            override;
    void on_mouse_down(uint button)       override;
    void on_mouse_move(const uint2d& pos) override;
    void on_exit() override;

    void _macro();
    void _toggle_flag();
    void _check_win();
    void _update_tagline();

public:
    minesweeper_app();
};
