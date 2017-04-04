#pragma once
#include <utility>

// forward decs
class character;
class team;

namespace events
{
	typedef void (characterEvent)(character*, team*, team*);
	typedef std::pair<float, characterEvent*> charEventProb;
	typedef void (moraleEvent)(character*, team*, team*);
	typedef std::pair<float, characterEvent*> moraleEventProb;
	typedef void (teamEvent)(team*, team*);
	typedef std::pair<float, teamEvent*> teamEventProb;

	extern charEventProb characterEvents[];
	extern moraleEventProb moraleEvents[];
	extern teamEventProb teamEvents[];

	characterEvent Rotation;
	characterEvent SplitPush;

	moraleEvent Encourage;
	moraleEvent Troll;
	moraleEvent Leave;
	moraleEvent ShitTalk;

	teamEvent Teamfight;
	teamEvent ObjectiveContest;
}

