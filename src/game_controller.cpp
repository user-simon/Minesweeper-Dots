#include "pch.h"
using json = nlohmann::json;

game_controller::game_controller()
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
			if (!data.count("times_played"))
			{
				m_difficulties.emplace_back(
					id++, data["name"], data["mines"], data["size"][0], data["size"][1]
				);
			}
			else
			{
				m_difficulties.emplace_back(
					id++, data["name"], data["mines"], data["size"][0], data["size"][1], data["pb"], data["times_played"], data["average"]
				);
			}
		}
		m_difficulty = &m_difficulties[main["default"]];
	}
	catch (...)
	{
		throw std::runtime_error("Couldn't read config data from file");
		return;
	}

	// create helper classes

	const VEC2U window_size = m_difficulty->size * design::cells::SIZE + VEC2U(0, design::ui::PANEL_HEIGHT);

	m_window = std::make_unique<application>(window_size, "Dots");
	m_grid   = std::make_unique<grid>();
	m_ui     = std::make_unique<ui>(m_window->client_size());

	// bind window callbacks

	using namespace std::placeholders;

	m_window->on_draw       = std::bind(&game_controller::_on_draw, this, _1);
	m_window->on_key_down   = std::bind(&game_controller::_on_key_down, this, _1);
	m_window->on_mouse_down = std::bind(&game_controller::_on_mouse_down, this, _1);
	m_window->on_mouse_move = std::bind(&game_controller::_on_mouse_move, this, _1);
	m_window->on_exit       = std::bind(&game_controller::_on_exit, this);

	// init grid

	m_grid->init(m_difficulty);
	_reset();

	// exec window
	
	m_window->exec();
}

void game_controller::_start()
{
	m_state.phase = GAME_STARTED;
	m_grid->move_mines(m_hovered_cell);
	m_state.start_time = std::time(0);
}

void game_controller::_end()
{
	m_state.phase = m_state.cells_left ? GAME_LOST : GAME_WON;
	m_state.duration = std::time(0) - m_state.start_time;

	// update difficulty statistics if the game was won
	if (m_state.phase == GAME_WON)
	{
		if (m_state.duration < m_difficulty->average)
			m_state.beat_average = true;

		// update average (https://math.stackexchange.com/questions/22348/how-to-add-and-subtract-values-from-an-average)
		{
			// limit times played to 10 for the calculation so new scores aren't weighed too low
			const int times_played = std::min(++m_difficulty->times_played, 10U);

			// round difference to nearest whole second
			const int delta = std::round((float)m_state.duration - m_difficulty->average);

			m_difficulty->average += delta / times_played;
		}
		
		// update high-score if new one is set
		if (m_state.duration < m_difficulty->pb)
		{
			m_difficulty->pb = m_state.duration;
			m_state.beat_record = true;
		}
	}
}

void game_controller::_reset()
{
	m_state.phase        = GAME_READY;
	m_state.cells_left   = m_difficulty->cell_count - m_difficulty->mines;
	m_state.flags_left   = m_difficulty->mines;
	m_state.beat_record  = false;
	m_state.beat_average = false;
	
	_update_tagline();
	m_grid->reset();
}

void game_controller::_on_mouse_move(VEC2I pos)
{
	if (pos.y < design::ui::PANEL_HEIGHT)
	{
		m_hovered_cell = nullptr;
		return;
	}
	pos.y -= design::ui::PANEL_HEIGHT;
	pos /= design::cells::SIZE;

	m_hovered_cell = m_grid->get_cell(pos);
}

void game_controller::_on_mouse_down(UINT button)
{
	if (m_state.phase > GAME_STARTED)
	{
		_reset();
		return;
	}
	
	if (!m_hovered_cell)
		return;

	switch (button)
	{
		using BUTTON = sf::Mouse::Button;
		
		case BUTTON::Left: // open
		{
			if (m_state.phase == GAME_READY)
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
		case BUTTON::Right: // toggle flag
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

void game_controller::_on_key_down(UINT key)
{	
	switch (key)
	{
		using KEY = sf::Keyboard::Key;
		
		case KEY::Space: // perform macro function
		{
			_macro();
			break;
		}
		case KEY::Escape: // restart
		{
			_reset();
			break;
		}
	}

	// set difficulty if a numbered key was pressed
	if (key >= 27 && key <= 36)
	{
		UINT id = key - 27;

		if (id < m_difficulties.size())
		{
			if (id != m_difficulty->id)
			{
				m_difficulty = &m_difficulties[id];
				m_window->resize(m_difficulty->size * design::cells::SIZE + VEC2U(0, design::ui::PANEL_HEIGHT));
				m_grid->init(m_difficulty);
				_update_tagline();
			}
			_reset();
		}
	}
}

void game_controller::_on_draw(sf::RenderTarget* ctx)
{
	UINT ellapsed_time = 0;

	// get game vars
	switch (m_state.phase)
	{
	case GAME_READY:
		ellapsed_time = 0;
		break;
	case GAME_STARTED:
		ellapsed_time = std::time(0) - m_state.start_time;
		break;
	case GAME_WON:
		m_state.flags_left = 0;
		[[fallthrough]];
	case GAME_LOST:
		ellapsed_time = m_state.duration;
		break;
	}
	m_grid->on_draw(m_state.phase, m_hovered_cell, ctx);
	m_ui->on_draw(ellapsed_time, m_state, ctx);
}

void game_controller::_on_exit()
{
	// serialize data to file

	json main;
	main["default"] = m_difficulty->id;
	
	for (difficulty_t& difficulty : m_difficulties)
	{
		main["difficulties"] +=
		{
			{ "name",         difficulty.name },
			{ "mines",        difficulty.mines },
			{ "size",         { difficulty.size.x, difficulty.size.y }},
			{ "pb",			  difficulty.pb },
			{ "times_played", difficulty.times_played },
			{ "average",      difficulty.average }
		};
	}
	std::ofstream stream("config.json");
	stream << std::setw(4) << main;
	stream.close();
}

void game_controller::_macro()
{
	if (!m_hovered_cell)
		return;

	if (m_state.phase > GAME_STARTED)
		return;

	if (m_hovered_cell->has(cell::DATA_OPEN))
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

void game_controller::_toggle_flag()
{
	if (m_hovered_cell->has(cell::DATA_OPEN))
		return;

	if (m_hovered_cell->toggle(cell::DATA_FLAG))
		m_state.flags_left--;
	else
		m_state.flags_left++;
}

void game_controller::_check_win()
{
	if (!m_state.cells_left)
		_end();
}

void game_controller::_update_tagline()
{
	std::string tagline = m_difficulty->name;

	if (m_difficulty->times_played)
		tagline += " (pb: " + std::to_string(m_difficulty->pb) + "s, avg: " + std::to_string(m_difficulty->average) + "s)";

	m_window->set_tagline(tagline);
}
