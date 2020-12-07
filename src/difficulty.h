#pragma once

struct difficulty_t
{
	const UINT id;
	const std::string name;
	const UINT mines;
	const VEC2U size;

	const UINT cell_count;

	int record;
	UINT times_played;
	int average;
	
	difficulty_t(UINT id, std::string name, UINT mines, UINT x, UINT y, UINT record, UINT times_played, UINT average)
		: name(name), id(id), mines(mines), size(x, y), cell_count(x * y), record(record), times_played(times_played), average(average)  {}

	difficulty_t(UINT id, std::string name, UINT mines, UINT x, UINT y)
		: difficulty_t(id, name, mines, x, y, -1, 0, -1) {}
};
