#pragma once

struct difficulty_t
{
	// config
	const uint id;
	const std::string name;
	const uint mines;
	const vec2u size;

	// helper
	const uint cell_count;

	// statistics
	float pb;
	uint completions;
	float avg;
	
	difficulty_t(uint id, std::string name, uint mines, uint w, uint h, float pb, uint completions, float avg)
		: name(name), id(id), mines(mines), size(w, h), cell_count(w * h), pb(pb), completions(completions), avg(avg) {}

	difficulty_t(uint id, std::string name, uint mines, uint w, uint h)
		: difficulty_t(id, name, mines, w, h, -1.f, 0.f, -1.f) {}
};
