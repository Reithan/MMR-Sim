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
	std::unique_ptr<player> mPlayer;

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
	
	void Death();
public:
	character(std::unique_ptr<player>&& my_player);
	~character();

	std::unique_ptr<player>& GetPlayer() { return mPlayer; }
	const RPSStat GetRPS() const { return counter; }
	const Role GetRole() const { return role; }
	const float GetLaneFavor() const { return lane_favor; }
	const float GetTeamFavor() const { return team_favor; }
	const float GetSkill() const;
	const float GetSynergy() const { return synergy; }
	const float GetPower();
	const float GetLevel() const { return level; }
	const short GetStreak() const { return streak; }

	void Pick(RPSStat rps, float lvf, Role lane) { counter = rps; lvf_balance = lvf; role = lane; }
	void Disconnect() { role = Role::DISCONNECTED; }
	void SetFavor(float new_lane_favor, float new_team_favor) { lane_favor = new_lane_favor; team_favor = new_team_favor; }
	void LaneEarnings(float exp, float gold);
	void Kill(character* victim);
	void Assist(character* victim, size_t num_assists);
};

short Counter(character* left, character* right);
