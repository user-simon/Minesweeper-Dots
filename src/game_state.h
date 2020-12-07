#pragma once

enum game_phase_t : UINT
{
	GAME_READY,
	GAME_STARTED,
	GAME_WON,
	GAME_LOST,
};

struct game_state_t
{
	game_phase_t phase;

	// started
	UINT cells_left;
	int flags_left;
	long long start_time;

	// over
	UINT duration;

	// won
	bool beat_record;
	bool beat_average;
};
