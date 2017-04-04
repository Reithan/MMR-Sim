#pragma once
#include "character.h"

class team
{
	float CC;
	float Tank;
	float Burst;
	float DPS;

	std::unique_ptr<character> character[5];
public:
	team();
	~team();

	const player* GetPlayer(const size_t player_num) const { return character[player_num]->GetPlayer(); }
};

