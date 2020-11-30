#include "pch.h"

application::application(VEC2U size, std::string_view window_name)
{
	m_client_size = size;
	m_window_name = window_name;

	m_ctx = std::make_unique<sf::RenderWindow>(sf::VideoMode(size.x, size.y), m_window_name, sf::Style::Close);
}

void application::exec()
{
	if (!m_ctx)
		return;

	while (m_ctx->isOpen())
	{
		sf::Event event;

		while (m_ctx->pollEvent(event))
			_handle_event(event);

		_draw();
	}
}

void application::resize(VEC2U size)
{
	m_client_size = size;
	m_ctx->setSize(sf::Vector2u(size.x, size.y));
	m_ctx->setView(sf::View({ 0, 0, (float)size.x, (float)size.y }));
}

VEC2U& application::client_size()
{
	return m_client_size;
}

void application::set_tagline(std::string_view tagline)
{
	m_ctx->setTitle(m_window_name + (tagline.length() ? " - " + std::string(tagline) : ""));
}

void application::_handle_event(sf::Event event)
{
	switch (event.type)
	{
		using EVENT = sf::Event;

		case EVENT::Closed:
		{
			m_ctx->close();

			if (on_exit)
				on_exit();
			break;
		}
		case EVENT::MouseMoved:
		{
			if (on_mouse_move && event.mouseMove.x < m_client_size.x && event.mouseMove.y < m_client_size.y)
				on_mouse_move(VEC2I(event.mouseMove.x, event.mouseMove.y));
			break;
		}
		case EVENT::MouseButtonPressed:
		{
			if (on_mouse_down)
				on_mouse_down(event.mouseButton.button);
			break;
		}
		case EVENT::KeyPressed:
		{
			if (on_key_down)
				on_key_down(event.key.code);
			break;
		}
			
	}
}

void application::_draw()
{
	if (on_draw)
	{
		m_ctx->clear();
		on_draw(m_ctx.get());
		m_ctx->display();
	}
}
