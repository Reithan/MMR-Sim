#pragma once
#include "character.h"

class team
{
public:
	enum Objective : unsigned short
	{
		// permanent objs
		TIER_ONE_TOWER,
		TIER_TWO_TOWER,
		TIER_THREE_TOWER,
		CORE,
		
		NUM_PERM_OBJECTIVES,

		// temp objs
		BUFF,
		TEAM_BUFF,
		TEAM_BOOST,

		NUM_OBJECTIVES
	};

private:
	float CC;
	float Tank;
	float Burst;
	float DPS;

	std::unique_ptr<character> characters[5];

	unsigned short objectiveUp[NUM_PERM_OBJECTIVES];

	void GenerateStats(bool present[5]);
public:
	team();
	~team();

	bool AddPlayer(std::unique_ptr<player>&& new_player);
	std::unique_ptr<player>& GetPlayer(const size_t player_num) { if (characters[player_num] == nullptr) return null_player;  return characters[player_num]->GetPlayer(); }
	character* GetCharacter(const size_t player_num) { return characters[player_num].get(); }
	const float GetAvgRating() const;
	void SortByRole();

	float GetCC() const { return CC; }
	float GetTank() const { return Tank; }
	float GetBurst() const { return Burst; }
	float GetDPS() const { return DPS; }
	float GetVision() const;
	unsigned short ObjectiveUp(Objective check) const { return objectiveUp[check]; }

	float OpposeStats(team* opp_for, bool present[2][5]);
	float LoseObjective();
	void RemoveObjective(Objective obj) { if (objectiveUp[obj] > 0) --objectiveUp[obj]; }
};

