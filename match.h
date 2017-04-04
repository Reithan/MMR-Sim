#pragma once
#include "team.h"

class match
{
	std::unique_ptr<team>	teams[2];
	float team_vision[2];

	enum Objective : unsigned short
	{
		// permanent objs
		TIER_ONE_TOWER,
		TIER_TWO_TOWER,
		TIER_THREE_TOWER,
		CORE,

		// temp objs
		BUFF,
		TEAM_BUFF,
		TEAM_BOOST,

		NUM_OBJECTIVES
	};
	unsigned short objectiveUp[NUM_OBJECTIVES][2];

	enum MatchPhase : unsigned short
	{
		DRAFT,
		EARLY,
		MID,
		LATE,

		NUM_PHASES
	};
	MatchPhase current_phase;
	short draft_pick;

	bool Pick();
public:
	match();
	~match();

	bool Update();
	unsigned short Winner() { return objectiveUp[CORE][0] ? 1 : 0; }

	team* GetTeam(const size_t team_num) { return teams[team_num].get(); }
	player* GetPlayer(const size_t team_num, const size_t player_num) { return teams[team_num]->GetPlayer(player_num); }
	character* GetCharacter(const size_t team_num, const size_t player_num) { return teams[team_num]->GetCharacter(player_num); }
};

