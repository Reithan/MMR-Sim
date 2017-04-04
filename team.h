#pragma once
#include "character.h"

class team
{
	float CC;
	float Tank;
	float Burst;
	float DPS;

	std::unique_ptr<character> characters[5];
public:
	team();
	~team();

	bool AddPlayer(player* const new_player);
	player* GetPlayer(const size_t player_num) { return characters[player_num]->GetPlayer(); }
	const float GetAvgRating() const;
};

