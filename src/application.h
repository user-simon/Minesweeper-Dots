#pragma once

class application
{
private:
	std::unique_ptr<sf::RenderWindow> m_ctx;
	VEC2U m_client_size;
	std::string m_window_name;

private:
	void _handle_event(sf::Event event_type);
	void _draw();

public:
	application(VEC2U size, std::string_view window_name);

	void exec();
	void resize(VEC2U size);

	VEC2U& client_size();
	void set_tagline(std::string_view tagline);

	std::function<void(VEC2U)> on_mouse_move;
	std::function<void(UINT)> on_mouse_down;
	std::function<void(UINT)> on_key_down;
	std::function<void(sf::RenderWindow*)> on_draw;
	std::function<void()> on_exit;
};
