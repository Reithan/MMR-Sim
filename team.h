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
	player* GetPlayer(const size_t player_num) { if (characters[player_num] == nullptr) return nullptr;  return characters[player_num]->GetPlayer(); }
	character* GetCharacter(const size_t player_num) { return characters[player_num].get(); }
	const float GetAvgRating() const;
	void SortByRole();
};

