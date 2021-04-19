#pragma once
#include <string>

#include <SFML/Graphics.hpp>
#include "vector_n/vector_n.h"

class sfml_app
{
private:
    sf::Color m_background_color;
    sf::RenderWindow m_window;

protected:
    std::string m_name;
    uint2d m_client_size;
    uint2d m_mouse_pos;

private:
    void _handle_event();
    void _draw_loop();
    bool _is_in_bounds(uint2d pos);

protected:
    virtual void _on_draw(sf::RenderTarget& ctx) {};
    virtual void _on_key_down(uint key)          {};
    virtual void _on_mouse_down(uint button)     {};
    virtual void _on_mouse_move(uint2d pos)      {};
    virtual void _on_exit()                      {};

public:
    sfml_app(const std::string& name, uint2d client_size, sf::Color background_color);

    void exec();
    void resize(uint2d size);
    uint2d client_size();
    void set_tagline(const std::string& tagline);
};
