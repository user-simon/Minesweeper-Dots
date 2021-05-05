#include "pch.h"
#include "sfml_app.h"

void sfml_app::init(const std::string& name, const uint2d& client_size, sf::Color background_color)
{
    m_name             = name;
    m_client_size      = client_size;
    m_background_color = background_color;

    m_window.emplace(sf::VideoMode(client_size.x, client_size.y), name);
}

bool sfml_app::exec()
{
    if (!m_window)
        return false;

    m_window->setActive(false);

    sf::Thread draw_thread(&sfml_app::_draw_loop, this);
    draw_thread.launch();
    
    while (m_window->isOpen())
    {
        _handle_event();
    }
    return true;
}

void sfml_app::_handle_event()
{
    sf::Event e;

    while (m_window->pollEvent(e))
    {
        switch (e.type)
        {
            using event = sf::Event;

            case event::Closed:
            {
                on_exit();
                m_window->close();
                break;
            }
            case event::Resized:
            {
                m_client_size = { e.size.width, e.size.height };
                break;
            }
            case event::KeyPressed:
            {
                on_key_down(e.key.code);
                break;
            }
            case event::MouseButtonPressed:
            {
                on_mouse_down(e.mouseButton.button);
                break;
            }
            case event::MouseMoved:
            {
                uint2d mouse_pos = { e.mouseMove.x, e.mouseMove.y };

                if (_is_in_bounds(mouse_pos))
                {
                    m_mouse_pos = mouse_pos;
                    on_mouse_move(m_mouse_pos);
                }
                break;
            }
        }
    }
}

void sfml_app::_draw_loop()
{
    m_window->setActive(true);

    while (m_window->isOpen())
    {
        m_window->clear(m_background_color);
        on_draw(*m_window);
        m_window->display();
    }
}

bool sfml_app::_is_in_bounds(const uint2d& pos)
{
    // since the coordinate are unsigned, we don't have to check for negative values
    return pos.x < m_client_size.x&& pos.y < m_client_size.y;
}

bool sfml_app::resize(const uint2d& size)
{
    if (!m_window)
        return false;

    m_client_size = size;
    m_window->setSize(size);
    m_window->setView(sf::View(sf::FloatRect(0, 0, size.x, size.y)));
    return true;
}

bool sfml_app::set_tagline(const std::string& tagline)
{
    if (!m_window)
        return false;

    m_window->setTitle(m_name + (tagline.length() ? " - " + tagline : ""));
    return true;
}

const uint2d& sfml_app::client_size() const
{
    return m_client_size;
}

const uint2d& sfml_app::mouse_pos() const
{
    return m_mouse_pos;
}
