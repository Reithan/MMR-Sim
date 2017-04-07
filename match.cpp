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
				for (size_t i = 0; i < 5; i++)
				{
					teams[1 - good_team]->GetCharacter(i)->GetPlayer()->UpdateTilt(0.25f);
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
			if (ievent->first[current_phase - 1] > rand() % 1000 / 1000.f)
			{
				size_t team_num = rand() % 2;
				ievent->second(teams[team_num]->GetCharacter(rand() % 5), teams[team_num].get(), teams[1 - team_num].get());
			}
		}
		for (auto ievent = events::teamEvents.begin(); ievent != events::teamEvents.end(); ++ievent)
		{
			if (ievent->first[current_phase - 1] > rand() % 1000 / 1000.f)
			{
				size_t team_num = rand() % 2;
				ievent->second(teams[team_num].get(), teams[1 - team_num].get());
			}
		}
	case match::DRAFT:
		// morale events
		for (auto ievent = events::moraleEvents.begin(); ievent != events::moraleEvents.end(); ++ievent)
		{
			size_t team_num = rand() % 2;
			size_t ichar = rand() % 5;
			float modifier = 1 + teams[team_num]->GetCharacter(ichar)->GetPlayer()->GetTilt() * (ievent->second == &events::Encourage ? 1 : -1);
			bool trigger = (ievent->first[current_phase] * modifier) > MultiRand(0.f, 1.f);
			if (trigger)
			{
				ievent->second(teams[team_num]->GetCharacter(ichar), teams[team_num].get(), teams[1 - team_num].get());
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
		{
			current_phase = match::EARLY;
			CalculateFavor();
		}
		break;
	case match::EARLY:
		FarmLanes(100.f);
		MakeRotations(1.f);
		current_phase = match::MID;
		break;
	case match::MID:
		FarmLanes(75.f);
		MakeRotations(1.5f);
		current_phase = match::LATE;
		break;
	case match::LATE:
		FarmLanes(0.25f);
		MakeRotations(2.f);
		break;
	}

	if (teams[0]->ObjectiveUp(team::Objective::CORE) + teams[1]->ObjectiveUp(team::Objective::CORE) < 2)
		return false;
	return true;
}

void match::CalculateFavor()
{
	teams[0]->SortByRole();
	teams[1]->SortByRole();
	for (size_t ichar1 = 0; ichar1 < 5; ++ichar1)
	{
		float lane_favor = 0.f,
			team_favor = 0.f;
		if (ichar1 < 3)
		{
			lane_favor = 0.2f * Counter(teams[0]->GetCharacter(ichar1), teams[1]->GetCharacter(ichar1));
		}
		else
		{
			lane_favor  = 0.05f * Counter(teams[0]->GetCharacter(3), teams[1]->GetCharacter(3));
			lane_favor += 0.05f * Counter(teams[0]->GetCharacter(3), teams[1]->GetCharacter(4));
			lane_favor += 0.05f * Counter(teams[0]->GetCharacter(4), teams[1]->GetCharacter(4));
			lane_favor += 0.05f * Counter(teams[0]->GetCharacter(4), teams[1]->GetCharacter(3));
		}
		for (size_t ichar2 = 0; ichar2 < 5; ++ichar2)
		{
			team_favor += 0.05f * Counter(teams[0]->GetCharacter(ichar1), teams[1]->GetCharacter(ichar2));
		}
		teams[0]->GetCharacter(ichar1)->SetFavor(1 + lane_favor, 1 + team_favor);
		teams[1]->GetCharacter(ichar1)->SetFavor(1 - lane_favor, 1 - team_favor);
	}
}

void match::FarmLanes(float amount)
{
	for (size_t ichar = 0; ichar < 5; ++ichar)
	{
		float result = MultiRand(0.f, 1.f, 4);
		auto char1 = teams[0]->GetCharacter(ichar);
		auto char2 = teams[1]->GetCharacter(ichar);
		result = std::max<float>(0.f, std::min<float>(1.f,
			result + char1->GetPower() * char1->GetLaneFavor() - char2->GetPower() * char2->GetLaneFavor()));

		switch (Role(ichar))
		{
		case OFFLANE:
		case MIDLANE:
			char1->LaneEarnings(amount * result, amount * result);
			char2->LaneEarnings(amount * (1 - result), amount * (1 - result));
			break;
		case JUNGLE:
			char1->LaneEarnings(0.75f * amount * result, 0.6f * amount * result);
			char2->LaneEarnings(0.75f * amount * (1 - result), 0.6f * amount * (1 - result));
			break;
		case CARRY:
			char1->LaneEarnings(0.65f * amount * result, 0.9f * amount * result);
			char2->LaneEarnings(0.65f * amount * (1 - result), 0.9f * amount * (1 - result));
			break;
		case SUPPORT:
			char1->LaneEarnings(0.65f * amount * result, 0.1f * amount * result);
			char2->LaneEarnings(0.65f * amount * (1 - result), 0.1f * amount * (1 - result));
			break;
		}

		for (float i = 0.f; i < 0.4f; i += 0.1f)
		{
			if (result >= 1.f - i)
				char1->Kill(char2);
			else if (result <= i)
				char2->Kill(char1);
		}
	}
}

void match::MakeRotations(float scale)
{
	for (size_t iteam = 0; iteam < 2; ++iteam)
	{
		for (size_t ichar = 0; ichar < 5; ++ichar)
		{
			short ganks = 0;
			auto char_ptr = teams[iteam]->GetCharacter(ichar);
			switch (Role(ichar))
			{
			case OFFLANE:
			case SUPPORT:
				ganks = std::max<short>(0, (short)MultiRand(-1.f, 2.f, 2));
				break;
			case MIDLANE:
				ganks = std::max<short>(0, (short)MultiRand(0.f, 3.f, 2));
				break;
			case JUNGLE:
				ganks = std::max<short>(0, (short)MultiRand(0.f, 4.f, 2));
				break;
			case CARRY:
				ganks = std::max<short>(0, (short)MultiRand(0.f, 1.f, 2));
				break;
			}
			ganks = short(scale * ganks * char_ptr->GetPower() * 5.f);
			for (short i = 0; i < ganks; ++i)
				events::Rotation(char_ptr, teams[iteam].get(), teams[1 - iteam].get());

			if (MultiRand(0.f, 1.f) - char_ptr->GetPower() < 0.05f)
				events::SplitPush(char_ptr, teams[iteam].get(), teams[1 - iteam].get());
		}
		if (MultiRand(0.f, 1.f) < 0.05f)
			events::ObjectiveContest(teams[iteam].get(), teams[1 - iteam].get());
	}
}
