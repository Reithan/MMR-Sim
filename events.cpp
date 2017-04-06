#include "stdafx.h"
#include "events.h"
#include "team.h"
#include "player.h"

namespace events
{
	const extern std::vector<charEventProb> characterEvents = {
		charEventProb({ 0.01f, 0.25f, 0.6f }, &Rotation),
		charEventProb({ 0.001f, 0.05f, 0.25f }, &SplitPush),
	};
	const extern std::vector<moraleEventProb> moraleEvents = {
		moraleEventProb({ 0.01f, 0.02f, 0.01f, 0.005f }, &Encourage),
		moraleEventProb({ 0.005f, 0.005f, 0.01f, 0.01f }, &Troll),
		moraleEventProb({ 0.001f, 0.001f, 0.005f, 0.01f }, &Leave),
		moraleEventProb({ -1.f, 0.01f, 0.05f, 0.1f }, &ShitTalk),
	};
	const extern std::vector<teamEventProb> teamEvents = {
		teamEventProb({ 0.01f,0.75f,1.f }, &Teamfight),
		teamEventProb({ 0.01f,0.75f,1.f }, &ObjectiveContest),
	};

	// Character-Driven Events
	void Rotation(character* actor, team* actor_team, team* opp_team)
	{
		Role act_role = actor->GetRole();
		float blufor, oppfor;
		Role target = Role((size_t)MultiRand(0.f, 5.f));
		while(target == act_role ||
			(target == Role::CARRY && act_role == Role::SUPPORT) ||
			(target == Role::SUPPORT && act_role == Role::CARRY))
			target = Role((size_t)MultiRand(0.f, 5.f));
		if (target == Role::CARRY || target == Role::SUPPORT)
		{
			blufor = actor->GetPower() + actor_team->GetCharacter(Role::CARRY)->GetPower() + actor_team->GetCharacter(Role::SUPPORT)->GetPower();
			oppfor = opp_team->GetCharacter(Role::CARRY)->GetPower() + opp_team->GetCharacter(Role::SUPPORT)->GetPower();
		}
		else
		{
			blufor = actor->GetPower() + actor_team->GetCharacter(target)->GetPower();
			oppfor = opp_team->GetCharacter(target)->GetPower();
		}

		float result = (blufor - oppfor) + MultiRand(0.5f * -opp_team->GetVision(), 0.5f * actor_team->GetVision(), 2);
		if (result > 0.25f)
		{
			if(target == Role::CARRY || target == Role::SUPPORT)
			{
				actor_team->GetCharacter(target)->Assist(opp_team->GetCharacter(target), 2);
				actor_team->GetCharacter(Role::SUPPORT)->Assist(opp_team->GetCharacter(target), 2);
			}
			else
			{
				actor_team->GetCharacter(target)->Assist(opp_team->GetCharacter(target), 1);
			}
			actor->Kill(opp_team->GetCharacter(target));
		}
		else if (result > 0.1f)
		{
			opp_team->GetCharacter(target)->LaneEarnings(-10.f, -10.f);
			actor_team->GetCharacter(target)->LaneEarnings(10.f, 10.f);
			actor->LaneEarnings(-10.f, -10.f);
		}
		else if (result < -0.25f)
		{
			if (target == Role::CARRY || target == Role::SUPPORT)
			{
				opp_team->GetCharacter(Role::SUPPORT)->Assist(opp_team->GetCharacter(target), 2);
			}
			opp_team->GetCharacter(target)->Kill(actor);
		}
		else if (result < -0.1f)
		{
			opp_team->GetCharacter(target)->LaneEarnings(10.f, 10.f);
			actor_team->GetCharacter(target)->LaneEarnings(-10.f, -10.f);
			actor->LaneEarnings(-10.f, -10.f);
		}
	}
	void SplitPush(character* actor, team* actor_team, team* opp_team)
	{
		float blu_vision = actor_team->GetVision();
		float opp_vision = opp_team->GetVision();
		float sneaky = MultiRand(-opp_vision, blu_vision, 2);
		if (sneaky + actor->GetPower() > 1.f)
		{
			float gain = opp_team->LoseObjective();
			for (size_t ichar = 0; ichar < 5; ++ichar)
			{
				actor_team->GetCharacter(ichar)->LaneEarnings(gain, gain);
			}
		}
		else
		{
			Rotation(actor, actor_team, opp_team);
		}
	}

	// Character Morale Events
	void Encourage(character* actor, team* actor_team, team* opp_team)
	{
		for (size_t iplayer = 0; iplayer < 5; ++iplayer)
		{
			auto char_ptr = actor_team->GetCharacter(iplayer);
			if (char_ptr->GetRole() == Role::DISCONNECTED)
				continue;
			if (actor == char_ptr)
				char_ptr->GetPlayer()->UpdateTilt(MultiRand(0.f,0.05f));
			else
				char_ptr->GetPlayer()->UpdateTilt(MultiRand(-0.05f, 0.15f, 2));
		}
	}
	void Troll(character* actor, team* actor_team, team* opp_team)
	{
		for (size_t iplayer = 0; iplayer < 5; ++iplayer)
		{
			auto char_ptr = actor_team->GetCharacter(iplayer);
			if (char_ptr->GetRole() == Role::DISCONNECTED)
				continue;
			if (actor == char_ptr)
				char_ptr->GetPlayer()->UpdateTilt(MultiRand(-0.1f, 0.1f, 2));
			else
				char_ptr->GetPlayer()->UpdateTilt(MultiRand(-0.15f, 0.05f));
		}
	}
	void Leave(character* actor, team* actor_team, team* opp_team)
	{
		actor->GetPlayer()->UpdateTilt(MultiRand(-0.05f, 0.05f));
		actor->Disconnect();

		short num_DC[2] = { 0 };
		for (size_t iplayer = 0; iplayer < 5; ++iplayer)
		{
			auto char_ptr = actor_team->GetCharacter(iplayer);
			if (char_ptr->GetRole() != Role::DISCONNECTED)
				char_ptr->GetPlayer()->UpdateTilt(MultiRand(-0.1f, 0.f));
			else
				++num_DC[0];

			char_ptr = opp_team->GetCharacter(iplayer);
			if (char_ptr->GetRole() == Role::DISCONNECTED)
				char_ptr->GetPlayer()->UpdateTilt(MultiRand(-0.1f, 0.05f, 2));
			else
				++num_DC[1];
		}

		if (num_DC[0] == 5)
			actor_team->RemoveObjective(team::Objective::CORE);
		else if (num_DC[1] == 5)
			opp_team->RemoveObjective(team::Objective::CORE);
	}
	void ShitTalk(character* actor, team* actor_team, team* opp_team)
	{
		for (size_t iplayer = 0; iplayer < 5; ++iplayer)
		{
			auto char_ptr = actor_team->GetCharacter(iplayer);
			if (char_ptr->GetRole() == Role::DISCONNECTED)
				continue;
			char_ptr->GetPlayer()->UpdateTilt(MultiRand(-0.05f, 0.1f));

			char_ptr = opp_team->GetCharacter(iplayer);
			if (char_ptr->GetRole() == Role::DISCONNECTED)
				continue;
			char_ptr->GetPlayer()->UpdateTilt(MultiRand(-0.15f, 0.05f, 2));
		}
	}

	// Team-Driven Events
	void Teamfight(team* actor_team, team* opp_team)
	{
		short num_present[2] = { 0, };
		bool present[2][5] = { 0, };
		float blu_vision = actor_team->GetVision();
		float opp_vision = opp_team->GetVision();
		character* blu_split = nullptr, *opp_split = nullptr;
		for (size_t ichar = 0; ichar < 5; ++ichar)
		{
			if (MultiRand(-opp_vision, blu_vision, 2) > -0.25f)
			{
				present[0][ichar] = true;
				++num_present[0];
			}
			else if (MultiRand(0.f, 1.f) < 0.1f)
				blu_split = actor_team->GetCharacter(ichar);

			if (MultiRand(-blu_vision, opp_vision, 2) > -0.25f)
			{
				present[1][ichar] = true;
				++num_present[1];
			}
			else if (MultiRand(0.f, 1.f) < 0.1f)
				opp_split = opp_team->GetCharacter(ichar);
		}

		if (blu_split != nullptr)
			SplitPush(blu_split, actor_team, opp_team);
		if (opp_split != nullptr)
			SplitPush(opp_split, opp_team, actor_team);

		float result = actor_team->OpposeStats(opp_team, present) + MultiRand(-0.1f, 0.1f, 2);
		result = 0.5f * (result + 1);

		for (size_t ichar = 0; ichar < 5; ++ichar)
		{
			character* victim = nullptr;
			size_t num_assists = 0;
			if (present[1][ichar] && MultiRand(0.f, 1.f, 2) < result)
			{
				victim = opp_team->GetCharacter(ichar);
				num_assists = std::max<short>(0, (short)MultiRand(0.f, 2.f * num_present[0], 3) - num_present[0]);
				for (size_t ihelper = (size_t)MultiRand(0, 4), i = 0; i < num_assists; ++i)
				{
					actor_team->GetCharacter(ihelper)->Assist(victim, num_assists);
					ihelper = (ihelper + 1) % 5;
				}
				actor_team->GetCharacter((size_t)MultiRand(0.f, 5.f))->Kill(victim);
			}
			if (present[0][ichar] && MultiRand(0.f, 1.f, 2) > result)
			{
				victim = actor_team->GetCharacter(ichar);
				num_assists = std::max<short>(0, (short)MultiRand(0.f, 2.f * num_present[1], 3) - num_present[1]);
				for (size_t ihelper = (size_t)MultiRand(0, 4), i = 0; i < num_assists; ++i)
				{
					opp_team->GetCharacter(ihelper)->Assist(victim, num_assists);
					ihelper = (ihelper + 1) % 5;
				}
				opp_team->GetCharacter((size_t)MultiRand(0.f, 5.f))->Kill(victim);
			}
		}

		if (result > 0.75f)
		{
			float gain = opp_team->LoseObjective();
			for (size_t ichar = 0; ichar < 5; ++ichar)
			{
				actor_team->GetCharacter(ichar)->LaneEarnings(gain, gain);
			}
		}
		else if (result < 0.25f)
		{
			ObjectiveContest(opp_team, actor_team);
		}
	}
	void ObjectiveContest(team* actor_team, team* opp_team)
	{
		float blu_vision = actor_team->GetVision();
		float opp_vision = opp_team->GetVision();
		float sneaky = MultiRand(-opp_vision, blu_vision, 2);
		if (sneaky > 0.6f)
		{
			float gain = opp_team->LoseObjective();
			for (size_t ichar = 0; ichar < 5; ++ichar)
			{
				actor_team->GetCharacter(ichar)->LaneEarnings(gain, gain);
			}
		}
		else if (sneaky > 0.25f)
		{
			SplitPush(actor_team->GetCharacter((size_t)MultiRand(0.f, 5.f)), actor_team, opp_team);
		}
		else if (sneaky > -0.1f)
		{
			Teamfight(actor_team, opp_team);
		}
		else if (sneaky > -0.25f)
		{
			Teamfight(actor_team, opp_team);
		}
		else
		{
			Rotation(opp_team->GetCharacter((size_t)MultiRand(0.f, 5.f)), opp_team, actor_team);
		}
	}
}
