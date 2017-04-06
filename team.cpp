#include "stdafx.h"
#include "team.h"
#include "player.h"

team::team()
{
	for (unsigned short iobj = 0; iobj < Objective::NUM_PERM_OBJECTIVES; ++iobj)
	{
		switch (iobj)
		{
		case team::TIER_ONE_TOWER:
		case team::TIER_TWO_TOWER:
		case team::TIER_THREE_TOWER:
		case team::BUFF:
			objectiveUp[iobj] = 3;
			break;
		case team::CORE:
		case team::TEAM_BUFF:
		case team::TEAM_BOOST:
			objectiveUp[iobj] = 1;
			break;
		}
	}
}


team::~team()
{
}

bool team::AddPlayer(std::unique_ptr<player>&& new_player)
{
	for (size_t i = 0; i < 5; ++i)
	{
		if (nullptr == characters[i])
		{
			characters[i].reset(new character(std::move(new_player)));
			return true;
		}
	}
	return false;
}

const float team::GetAvgRating() const
{
	float total = 0;
	unsigned short num_players = 0;
	for (size_t i = 0; i < 5; ++i)
	{
		if (characters[i] != nullptr)
		{
			total += characters[i]->GetPlayer()->GetRating();
			++num_players;
		}
	}
	return total / num_players;
}

void team::SortByRole()
{
	std::sort(&characters[0], &characters[5], [](std::unique_ptr<character>& left, std::unique_ptr<character>& right)->bool
		{
			return left->GetRole() < right->GetRole();
		}
	);
}

float team::GetVision() const
{
	float vision = 0.f;
	for (size_t ichar = 0; ichar < 5; ++ichar)
	{
		if (ichar == Role::SUPPORT)
			vision += 0.4f * characters[ichar]->GetPower();
		if (ichar != Role::CARRY)
			vision += 0.2f * characters[ichar]->GetPower();
	}
	return vision;
}

void team::GenerateStats(bool present[5])
{
	CC = Tank = Burst = DPS = 0.f;

	float power = 0.f;
	if (present[Role::OFFLANE])
	{
		power = characters[Role::OFFLANE]->GetPower();
		CC += 0.3f * power;
		Tank += 0.4f * power;
		Burst += 0.05f * power;
		DPS += 0.2f * power;
	}
	if (present[Role::OFFLANE])
	{
		power = characters[Role::MIDLANE]->GetPower();
		CC += 0.2f * power;
		Tank += 0.05f * power;
		Burst += 0.4f * power;
		DPS += 0.05f * power;
	}
	if (present[Role::OFFLANE])
	{
		power = characters[Role::JUNGLE]->GetPower();
		CC += 0.05f * power;
		Tank += 0.3f * power;
		Burst += 0.3f * power;
		DPS += 0.3f * power;
	}
	if (present[Role::OFFLANE])
	{
		power = characters[Role::CARRY]->GetPower();
		CC += 0.05f * power;
		Tank += 0.05f * power;
		Burst += 0.2f * power;
		DPS += 0.4f * power;
	}
	if (present[Role::OFFLANE])
	{
		power = characters[Role::SUPPORT]->GetPower();
		CC += 0.4f * power;
		Tank += 0.2f * power;
		Burst += 0.05f * power;
		DPS += 0.05f * power;
	}
}

float team::OpposeStats(team* opp_for, bool present[2][5])
{
	GenerateStats(present[0]);
	opp_for->GenerateStats(present[1]);
	return 0.5f * (DPS - opp_for->CC + Burst - opp_for->Tank);
}

float team::LoseObjective()
{
	if (objectiveUp[CORE] == 0)
		return 0.f;

	Objective target = Objective::NUM_OBJECTIVES;
	while (true)
	{
		target = Objective((short)MultiRand(0.f, Objective::NUM_OBJECTIVES - 1.f));
		switch (target)
		{
		case team::TIER_ONE_TOWER:
			if (objectiveUp[TIER_ONE_TOWER] > 0)
			{
				--objectiveUp[target];
				return 0.15f;
			}
			break;
		case team::TIER_TWO_TOWER:
			if (objectiveUp[TIER_TWO_TOWER] > 0 && objectiveUp[TIER_ONE_TOWER] < objectiveUp[TIER_TWO_TOWER])
			{
				--objectiveUp[target];
				return 0.15f;
			}
			break;
		case team::TIER_THREE_TOWER:
			if (objectiveUp[TIER_THREE_TOWER] > 0 && objectiveUp[TIER_TWO_TOWER] < objectiveUp[TIER_THREE_TOWER])
			{
				--objectiveUp[target];
				return 0.2f;
			}
			break;
		case team::CORE:
			if(objectiveUp[CORE] > 0 && objectiveUp[TIER_THREE_TOWER] < 3)
			{
				--objectiveUp[target];
				return 0.f;
			}
			break;
		case team::BUFF:
			return 0.05f;
		case team::TEAM_BUFF:
			return 0.25f;
		case team::TEAM_BOOST:
			return 0.1f;
		}
	}
}
