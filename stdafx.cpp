// stdafx.cpp : source file that includes just the standard includes
// MMRSim.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

extern std::unique_ptr<player> null_player = nullptr;

float MultiRand(float min, float max, size_t multi)
{
	if (max - min == 0.f)
		return min;

	float total = 0.f;
	for (size_t i = 0; i < multi; i++)
		total += rand() % int((max - min) * 10000) + (min * 10000);
	return total / (multi * 10000.f);
}
