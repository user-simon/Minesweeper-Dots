#pragma once
#include <string>

#include "vector_n/vector_n.h"

struct difficulty
{
	// config
	const uint id;
	const std::string name;
	const uint mines;
	const uint2d size;

	// helper
	const uint cell_count;

	// statistics
	float pb;
	uint completions;
	float avg;
	
	difficulty(uint id, const std::string& name, uint mines, uint w, uint h, float pb, uint completions, float avg)
		: name(name), id(id), mines(mines), size(w, h), cell_count(w * h), pb(pb), completions(completions), avg(avg) {}

	difficulty(uint id, const std::string& name, uint mines, uint w, uint h)
		: difficulty(id, name, mines, w, h, -1.f, 0.f, -1.f) {}
};
