#include "stdafx.h"
#include "events.h"

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
	{}
	void Troll(character* actor, team* actor_team, team* opp_team)
	{}
	void Leave(character* actor, team* actor_team, team* opp_team)
	{}
	void ShitTalk(character* actor, team* actor_team, team* opp_team)
	{}

	void Teamfight(team* actor_team, team* opp_team)
	{}
	void ObjectiveContest(team* actor_team, team* opp_team)
	{}
}
