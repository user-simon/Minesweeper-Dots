#pragma once

struct difficulty_t
{
	// config
	const UINT id;
	const std::string name;
	const UINT mines;
	const VEC2U size;

	// helper
	const UINT cell_count;

	// statistics
	int record;
	UINT times_played;
	int average;
	
	difficulty_t(UINT id, std::string name, UINT mines, UINT w, UINT h, UINT record, UINT times_played, UINT average)
		: name(name), id(id), mines(mines), size(w, h), cell_count(w * h), record(record), times_played(times_played), average(average)  {}

	difficulty_t(UINT id, std::string name, UINT mines, UINT w, UINT h)
		: difficulty_t(id, name, mines, w, h, -1, 0, -1) {}
};
