#pragma once

enum GAME_PHASE : uint
{
	GAME_READY,
	GAME_STARTED,
	GAME_WON,
	GAME_LOST,
};

struct game_state_t
{
	GAME_PHASE phase;

	// started
	uint cells_left;
	int flags_left;
	std::chrono::system_clock::time_point start_time;

	// over
	float duration;

	// won
	bool beat_pb;
	bool beat_avg;

	void update_duration()
	{
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count() / 1000.f;
	}
};
