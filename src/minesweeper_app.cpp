#include "pch.h"
#include "minesweeper_app.h"
using json = nlohmann::json;

minesweeper_app::minesweeper_app()
    : sfml_app("Dots", uint2d::ZERO, sf::Color::Black), m_ui(m_client_size)
{
    // deserialize config data from file
    
    try
    {
        json main;
        std::ifstream stream("config.json");
        stream >> main;
        stream.close();

        int id = 0;
        
        for (json& data : main["difficulties"])
        {
            // use different constructor if data doesn't contain statistics
            if (data.count("stats"))
            {
                json& stats = data["stats"];

                m_difficulties.emplace_back(
                    id++, data["name"], data["mines"], data["size"][0], data["size"][1], stats["pb"], stats["completions"], stats["average"]
                );
            }
            else
            {
                m_difficulties.emplace_back(
                    id++, data["name"], data["mines"], data["size"][0], data["size"][1]
                );
            }
        }
        m_difficulty = &m_difficulties[main["default"]];
    }
    catch (...)
    {
        throw std::runtime_error("Couldn't read config data from file");
    }

    // create helper classes

    const uint2d window_size = m_difficulty->size * design::cells::SIZE + uint2d(0, design::ui::PANEL_HEIGHT);

    resize(window_size);

    // init grid

    m_grid.init(m_difficulty->size);
    _reset();
}

void minesweeper_app::_start()
{
    m_state.phase = PHASE::STARTED;
    m_grid.populate(m_difficulty->mines, m_hovered_cell->pos);
    m_state.clock.restart();
}

void minesweeper_app::_end()
{
    m_state.phase = m_state.cells_left ? PHASE::LOST : PHASE::WON;

    // update difficulty statistics if the game was won
    if (m_state.phase == PHASE::WON)
    {
        m_state.flags_left = 0;

        if (m_state.duration < m_difficulty->avg)
            m_state.beat_avg = true;

        // update average (https://math.stackexchange.com/questions/22348/how-to-add-and-subtract-values-from-an-average)
        {
            // limit times played to 5 for the calculation so new scores aren't weighed too low
            const int completions = std::min(++m_difficulty->completions, 5U);

            // calculate new average
            m_difficulty->avg += (m_state.duration - m_difficulty->avg) / completions;
        }
        
        // update high-score if new one is set
        if (m_state.duration < m_difficulty->pb || m_difficulty->pb < 0)
        {
            m_difficulty->pb = m_state.duration;
            m_state.beat_pb = true;
        }
    }
}

void minesweeper_app::_reset()
{
    m_state.phase      = PHASE::READY;
    m_state.cells_left = m_difficulty->cell_count - m_difficulty->mines;
    m_state.flags_left = m_difficulty->mines;
    m_state.duration   = 0;
    m_state.beat_pb    = false;
    m_state.beat_avg   = false;
    
    _update_tagline();
    m_grid.reset();
}

void minesweeper_app::_on_draw(sf::RenderTarget& ctx)
{
    // update duration only while the game is running
    if (m_state.phase == PHASE::STARTED)
        m_state.duration = m_state.clock.getElapsedTime().asSeconds();

    // draw elements
    m_grid.on_draw(m_state.phase, m_hovered_cell, ctx);
    m_ui.on_draw(m_state, ctx);
}

void minesweeper_app::_on_key_down(uint key)
{
    switch (key)
    {
        using key = sf::Keyboard::Key;

        case key::Space: // perform macro function
        {
            _macro();
            break;
        }
        case key::Escape: // restart
        {
            _reset();
            break;
        }
    }

    // set difficulty if a numbered key was pressed
    if (key >= 27 && key <= 36)
    {
        uint id = key - 27;

        if (id < m_difficulties.size())
        {
            if (id != m_difficulty->id)
            {
                m_difficulty = &m_difficulties[id];
                resize(m_difficulty->size * design::cells::SIZE + uint2d(0, design::ui::PANEL_HEIGHT));
                m_grid.init(m_difficulty->size);
                _update_tagline();
            }
            _reset();
        }
    }
}

void minesweeper_app::_on_mouse_down(uint button)
{
    if (m_state.phase > PHASE::STARTED)
    {
        _reset();
        return;
    }
    
    if (!m_hovered_cell)
        return;

    switch (button)
    {
        using button = sf::Mouse::Button;
        
        case button::Left: // open
        {
            if (m_state.phase == PHASE::READY)
                _start();

            if (int buf; (buf = m_hovered_cell->open()) >= 0)
            {
                m_state.cells_left -= buf;
                _check_win();
                return;
            }
            else
            {
                _end();
            }
            break;
        }
        case button::Right: // toggle flag
        {
            _toggle_flag();
            break;
        }
        default:// perform the macro function
        {
            _macro();
            break;
        }
    }
}

void minesweeper_app::_on_mouse_move(uint2d pos)
{
    if (pos.y < design::ui::PANEL_HEIGHT)
    {
        m_hovered_cell = nullptr;
        return;
    }
    pos.y -= design::ui::PANEL_HEIGHT;
    pos /= design::cells::SIZE;

    m_hovered_cell = m_grid.get_cell(pos);
}

void minesweeper_app::_on_exit()
{
    // serialize data to file

    json main;
    main["default"] = m_difficulty->id;

    json& difficulties = main["difficulties"];
    
    for (difficulty& difficulty : m_difficulties)
    {
        json data = 
        {
            { "name",   difficulty.name                        },
            { "mines",  difficulty.mines                       },
            { "size", { difficulty.size.x, difficulty.size.y } }
        };

        if (difficulty.completions)
        {
            data +=
            {
                "stats",
                {
                    { "pb",          difficulty.pb          },
                    { "completions", difficulty.completions },
                    { "average",     difficulty.avg         }
                }
            };
        }
        difficulties += data;
    }
    std::ofstream stream("config.json");
    stream << std::setw(4) << main;
    stream.close();
}

void minesweeper_app::_macro()
{
    if (!m_hovered_cell)
        return;

    if (m_state.phase > PHASE::STARTED)
        return;

    if (m_hovered_cell->is_open)
    {
        if (int buf; (buf = m_hovered_cell->open_neighbours()) >= 0)
        {
            m_state.cells_left -= buf;
            _check_win();
        }
        else
        {
            _end();
        }
    }
    else
    {
        _toggle_flag();
    }
}

void minesweeper_app::_toggle_flag()
{
    if (m_hovered_cell->is_open)
        return;

    // toggles m_hovered_cell->has_flag and updates
    // flags left accordingly
    if (m_hovered_cell->has_flag ^= true)
        m_state.flags_left--;
    else
        m_state.flags_left++;
}

void minesweeper_app::_check_win()
{
    if (!m_state.cells_left)
        _end();
}

void minesweeper_app::_update_tagline()
{
    std::string tagline = m_difficulty->name;

    const static auto format_float = [](float f)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << f;
        return stream.str();
    };

    if (m_difficulty->completions)
        tagline += " (pb: " + format_float(m_difficulty->pb) + "s, avg: " + format_float(m_difficulty->avg) + "s)";

    set_tagline(tagline);
}
