#pragma once
// forward decs
class character;
class team;

namespace events
{
	typedef void (characterEvent)(character*, team*, team*);
	typedef std::pair<std::vector<float>, characterEvent*> charEventProb;
	
	typedef characterEvent moraleEvent;
	typedef std::pair<std::vector<float>, moraleEvent*> moraleEventProb;
	
	typedef void (teamEvent)(team*, team*);
	typedef std::pair<std::vector<float>, teamEvent*> teamEventProb;

	const extern std::vector<charEventProb> characterEvents;
	const extern std::vector<moraleEventProb> moraleEvents;
	const extern std::vector<teamEventProb> teamEvents;

	characterEvent Rotation;
	characterEvent SplitPush;

	moraleEvent Encourage;
	moraleEvent Troll;
	moraleEvent Leave;
	moraleEvent ShitTalk;

	teamEvent Teamfight;
	teamEvent ObjectiveContest;
}

