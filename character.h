#pragma once
#include "roles.h"
// forward dec
class player;

class character
{
public:
	enum RPSStat : unsigned short
	{
		ROCK,
		PAPER,
		SCISSORS,
		NUETRAL,

		NUM_RPS_STATS
	};
private:
	player* const mPlayer;

	RPSStat counter;
	float lvf_balance;
	Role role;
	float synergy;

	float level;
	float farm;
	unsigned short kda[3];
	short streak;
	float lane_favor;
	float team_favor;
public:
	character(player* const my_player);
	~character();

	player* GetPlayer() { return mPlayer; }
	RPSStat GetRPS() const { return counter; }
	Role GetRole() const { return role; }

	void Pick(RPSStat rps, float lvf, Role lane) { counter = rps; lvf_balance = lvf; role = lane; }
	void Disconnect() { role = Role::DISCONNECTED; }
	void SetFavor(float new_lane_favor, float new_team_favor) { lane_favor = new_lane_favor; team_favor = new_team_favor; }
};

short Counter(character* left, character* right);
