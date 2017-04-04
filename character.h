#pragma once
#include "roles.h"
// forward dec
class player;

class character
{
	player* const mPlayer;

	enum RPSStat : unsigned short
	{
		ROCK,
		PAPER,
		SCISSORS,
		NUETRAL,

		NUM_RPS_STATS
	};
	const RPSStat counter;
	const float lvf_balance;
	const Role role;
	const float synergy;

	float level;
	float farm;
	unsigned short kda[3];
	short streak;
public:
	character(player* const my_player);
	~character();

	player* GetPlayer() { return mPlayer; }
};

