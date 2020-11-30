#pragma once

struct difficulty_t
{
	UINT id;

	std::string name;
	UINT mines;
	VEC2U size;
	UINT cell_count;
	UINT record;
	
	difficulty_t() = default;
	difficulty_t(UINT id, std::string name, UINT mines, UINT x, UINT y, UINT record)
		: name(name), id(id), mines(mines), size(x, y), cell_count(x * y), record(record)  {}
};
