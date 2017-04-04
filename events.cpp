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
		moraleEventProb({ 0.0f, 0.01f, 0.05f, 0.1f }, &ShitTalk),
	};
	const extern std::vector<teamEventProb> teamEvents = {
		teamEventProb({ 0.01f,0.75f,1.f }, &Teamfight),
		teamEventProb({ 0.01f,0.75f,1.f }, &ObjectiveContest),
	};

	void Rotation(character* actor, team* actor_team, team* opp_team)
	{}
	void SplitPush(character* actor, team* actor_team, team* opp_team)
	{}

	void Encourage(character* actor, team* actor_team, team* opp_team)
	{
		for (size_t iplayer = 0; iplayer < 5; ++iplayer)
		{
			auto char_ptr = actor_team->GetCharacter(iplayer);
			if (actor == char_ptr)
				char_ptr->GetPlayer()->UpdateTilt((rand() % 50) / 1000.f);
			else
				char_ptr->GetPlayer()->UpdateTilt((rand() % 200 - 50) / 1000.f);
		}
	}
	void Troll(character* actor, team* actor_team, team* opp_team)
	{
		for (size_t iplayer = 0; iplayer < 5; ++iplayer)
		{
			auto char_ptr = actor_team->GetCharacter(iplayer);
			if (actor == char_ptr)
				char_ptr->GetPlayer()->UpdateTilt((rand() % 200 - 100) / 1000.f);
			else
				char_ptr->GetPlayer()->UpdateTilt((rand() % 50 - 200) / 1000.f);
		}
	}
	void Leave(character* actor, team* actor_team, team* opp_team)
	{}
	void ShitTalk(character* actor, team* actor_team, team* opp_team)
	{
		for (size_t iplayer = 0; iplayer < 5; ++iplayer)
		{
			auto char_ptr = actor_team->GetCharacter(iplayer);
			char_ptr->GetPlayer()->UpdateTilt((rand() % 150 - 50) / 1000.f);

			char_ptr = opp_team->GetCharacter(iplayer);
			char_ptr->GetPlayer()->UpdateTilt((rand() % 50 - 200) / 1000.f);
		}
	}

	void Teamfight(team* actor_team, team* opp_team)
	{}
	void ObjectiveContest(team* actor_team, team* opp_team)
	{}
}
