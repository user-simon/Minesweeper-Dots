#pragma once

class application
{
private:
	std::unique_ptr<sf::RenderWindow> m_ctx;
	vec2u m_client_size;
	std::string m_window_name;

private:
	void _handle_event(sf::Event event_type);
	void _draw();

public:
	application(vec2u size, std::string_view window_name);

	void exec();
	void resize(vec2u size);

	vec2u& client_size();
	void set_tagline(std::string_view tagline);

	std::function<void(vec2u)> on_mouse_move;
	std::function<void(uint)> on_mouse_down;
	std::function<void(uint)> on_key_down;
	std::function<void(sf::RenderTarget*)> on_draw;
	std::function<void()> on_exit;
};
