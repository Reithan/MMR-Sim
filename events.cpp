#include "stdafx.h"
#include "events.h"

namespace events
{
	const extern std::vector<charEventProb> characterEvents = {
		charEventProb({ 0.0f, 0.0f, 0.0f }, &Rotation),
		charEventProb({ 0.0f, 0.0f, 0.0f }, &SplitPush),
	};
	const extern std::vector<moraleEventProb> moraleEvents = {
		moraleEventProb({ 0.0f, 0.0f, 0.0f, 0.0f }, &Encourage),
		moraleEventProb({ 0.0f, 0.0f, 0.0f, 0.0f }, &Troll),
		moraleEventProb({ 0.0f, 0.0f, 0.0f, 0.0f }, &Leave),
		moraleEventProb({ 0.0f, 0.0f, 0.0f, 0.0f }, &ShitTalk),
	};
	const extern std::vector<teamEventProb> teamEvents = {
		teamEventProb({ 0.0f,0.0f,0.0f }, &Teamfight),
		teamEventProb({ 0.0f,0.0f,0.0f }, &ObjectiveContest),
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
