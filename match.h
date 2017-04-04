#pragma once
#include <memory>
// forward dec
class team;

class match
{
	std::unique_ptr<team>	teams[2];
	float team_vision[2];

	enum PermObjective : unsigned short
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
	unsigned short objectiveUp[NUM_OBJECTIVES];
public:
	match();
	~match();
};

