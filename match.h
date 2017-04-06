#pragma once
#include "team.h"

class match
{
	std::unique_ptr<team>	teams[2];

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
	unsigned short Winner() { return teams[0]->ObjectiveUp(team::Objective::CORE) ? 1 : 0; }

	team* GetTeam(const size_t team_num) { return teams[team_num].get(); }
	std::unique_ptr<player>& GetPlayer(const size_t team_num, const size_t player_num) { return teams[team_num]->GetPlayer(player_num); }
	character* GetCharacter(const size_t team_num, const size_t player_num) { return teams[team_num]->GetCharacter(player_num); }
	void CalculateFavor();
};

