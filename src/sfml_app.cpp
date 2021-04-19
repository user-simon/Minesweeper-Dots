#include "pch.h"
#include "sfml_app.h"

sfml_app::sfml_app(const std::string& name, uint2d client_size, sf::Color background_color)
    : m_name(name), m_client_size(client_size), m_background_color(background_color), m_window(sf::VideoMode(client_size.x, client_size.y), name)
{
    
}

void sfml_app::exec()
{
    m_window.setActive(false);

    sf::Thread draw_thread(&sfml_app::_draw_loop, this);
    draw_thread.launch();
    
    while (m_window.isOpen())
    {
        _handle_event();
    }
}

void sfml_app::resize(uint2d size)
{
    m_client_size = size;
    m_window.setSize(size);
    m_window.setView(sf::View(sf::FloatRect(0, 0, size.x, size.y)));
}

uint2d sfml_app::client_size()
{
    return m_client_size;
}

void sfml_app::set_tagline(const std::string& tagline)
{
    m_window.setTitle(m_name + (tagline.length() ? " - " + tagline : ""));
}

void sfml_app::_handle_event()
{
    sf::Event e;

    while (m_window.pollEvent(e))
    {
        switch (e.type)
        {
            using event = sf::Event;

            case event::Closed:
            {
                m_window.close();
                break;
            }
            case event::Resized:
            {
                m_client_size = { e.size.width, e.size.height };
                break;
            }
            case event::KeyPressed:
            {
                _on_key_down(e.key.code);
                break;
            }
            case event::MouseButtonPressed:
            {
                _on_mouse_down(e.mouseButton.button);
                break;
            }
            case event::MouseMoved:
            {
                uint2d mouse_pos = { e.mouseMove.x, e.mouseMove.y };

                if (_is_in_bounds(mouse_pos))
                {
                    m_mouse_pos = mouse_pos;
                    _on_mouse_move(m_mouse_pos);
                }
                break;
            }
        }
    }
}

void sfml_app::_draw_loop()
{
    m_window.setActive(true);

    while (m_window.isOpen())
    {
        m_window.clear(m_background_color);
        _on_draw(m_window);
        m_window.display();
    }
}

bool sfml_app::_is_in_bounds(uint2d pos)
{
    // since the coordinate are unsigned, we don't have to check for negative values
    return pos.x < m_client_size.x&& pos.y < m_client_size.y;
}
