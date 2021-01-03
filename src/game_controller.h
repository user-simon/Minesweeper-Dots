#pragma once

class game_controller
{
private:
	std::unique_ptr<application> m_window;
	std::unique_ptr<grid> m_grid;
	std::unique_ptr<ui> m_ui;

	std::vector<difficulty_t> m_difficulties;
	difficulty_t* m_difficulty;

	game_state_t m_state;

	cell* m_hovered_cell;

private:
	void _start();
	void _end();
	void _reset();

	void _on_mouse_move(vec2i pos);
	void _on_mouse_down(uint button);
	void _on_key_down(uint key);
	void _on_draw(sf::RenderTarget* ctx);
	void _on_exit();

	void _macro();
	void _toggle_flag();
	void _check_win();
	void _update_tagline();

public:
	game_controller();
};
