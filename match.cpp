#include "stdafx.h"
#include "match.h"
#include "events.h"
#include "player.h"

match::match() :
	draft_pick(-1)
{
	teams[0] = std::make_unique<team>();
	teams[1] = std::make_unique<team>();
	current_phase = MatchPhase::DRAFT;
}


match::~match()
{
}

bool match::Pick()
{
	if (draft_pick >= 0 && draft_pick < 10)
	{
		Role role = Role(rand() % Role::NUM_ROLES);
		float lvf = 0.5f;
		switch (role)
		{
		case OFFLANE:
		case MIDLANE:
		case JUNGLE:
			lvf = 0.6f;
			break;
		case CARRY:
			lvf = 0.3f;
			break;
		case SUPPORT:
			lvf = 0.7f;
			break;
		default:
			lvf = 0.5f;
			break;
		}

		bool picked = true;
		while (picked)
		{
			picked = false;
			for (size_t i = 0; i < 5; ++i)
			{
				if (teams[draft_pick % 2]->GetCharacter(i) != teams[draft_pick % 2]->GetCharacter(draft_pick / 2) &&
					role == teams[draft_pick % 2]->GetCharacter(i)->GetRole())
				{
					role = Role((role + 1) % Role::NUM_ROLES);
					picked = true;
				}
			}
		}

		character::RPSStat rps = character::RPSStat(rand() % character::RPSStat::NUM_RPS_STATS);

		teams[draft_pick % 2]->GetCharacter(draft_pick / 2)->Pick(rps, lvf, role);
		for (size_t i = 0; i < 5; ++i)
		{
			auto ichar = teams[draft_pick % 2]->GetCharacter(i);
			if (ichar->GetRole() != role)
				continue;

			size_t good_team = 0;
			character::RPSStat counter = ichar->GetRPS();
			if ((rps + 1) % 3 == counter)
				good_team = 1;
			else if (rps == (counter + 1) % 3)
				good_team = 0;
			else
				continue;

			// check for DC based on counter
			if (rand() % 100 < 5)
			{
				for (size_t i = 0; i < 10; i++)
				{
					teams[i / 5]->GetCharacter(i % 5)->GetPlayer()->UpdateTilt(0.25f);
				}
				return false;
			}
			// check for troll based on counter
			if (rand() % 100 < 10)
				events::Troll(teams[1 - good_team]->GetCharacter(rand() % 5), teams[1 - good_team].get(), teams[good_team].get());
			// check for encouragment based on counter
			if (rand() % 100 < 10)
				events::Encourage(teams[good_team]->GetCharacter(rand() % 5), teams[good_team].get(), teams[1 - good_team].get());
		}
	}
	++draft_pick;
	return true;
}

bool match::Update()
{
	// generate random events
	switch (current_phase)
	{
	case match::EARLY:
	case match::MID:
	case match::LATE:
		// non-morale events
		for (auto ievent = events::characterEvents.begin(); ievent != events::characterEvents.end(); ++ievent)
		{
			if (ievent->first[current_phase - 1] >= rand() % 1000 / 1000.f)
			{
				size_t team_num = rand() % 2;
				ievent->second(teams[team_num]->GetCharacter(rand() % 5), teams[team_num].get(), teams[1 - team_num].get());
			}
		}
		for (auto ievent = events::teamEvents.begin(); ievent != events::teamEvents.end(); ++ievent)
		{
			if (ievent->first[current_phase - 1] >= rand() % 1000 / 1000.f)
			{
				size_t team_num = rand() % 2;
				ievent->second(teams[team_num].get(), teams[1 - team_num].get());
			}
		}
	case match::DRAFT:
		// morale events
		for (auto ievent = events::moraleEvents.begin(); ievent != events::moraleEvents.end(); ++ievent)
		{
			if (ievent->first[current_phase] >= rand() % 1000 / 1000.f)
			{
				size_t team_num = rand() % 2;
				ievent->second(teams[team_num]->GetCharacter(rand() % 5), teams[team_num].get(), teams[1 - team_num].get());
			}
		}
		break;
	}

	// phase logic
	switch (current_phase)
	{
	case match::DRAFT:
		if (!Pick() || !Pick())
			return false;
		if (draft_pick >= 10)
			current_phase = match::EARLY;
		break;
	case match::EARLY:
		return false;
		break;
	case match::MID:
		break;
	case match::LATE:
		break;
	}

	if (objectiveUp[Objective::CORE][0] + objectiveUp[Objective::CORE][1] < 2)
		return false;
	return true;
}