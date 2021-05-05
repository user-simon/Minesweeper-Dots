#pragma once
#include <string>
#include <optional>

#include <SFML/Graphics.hpp>
#include "math_vector/math_vector.h"

class sfml_app
{
private:
    std::string m_name;
    uint2d m_client_size;
    uint2d m_mouse_pos;

    sf::Color m_background_color;
    std::optional<sf::RenderWindow> m_window;
    
private:
    void _handle_event();
    void _draw_loop();
    bool _is_in_bounds(const uint2d& pos);

protected:
    void init(const std::string& name, const uint2d& client_size, sf::Color background_color);

    virtual void on_draw(sf::RenderTarget& ctx)   {};
    virtual void on_key_down(uint key)            {};
    virtual void on_mouse_down(uint button)       {};
    virtual void on_mouse_move(const uint2d& pos) {};
    virtual void on_exit()                        {};

public:
    bool exec();
    
    bool resize(const uint2d& size);
    bool set_tagline(const std::string& tagline);
    
    const uint2d& client_size() const;
    const uint2d& mouse_pos() const;
};
