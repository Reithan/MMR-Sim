#include "stdafx.h"
#include "character.h"
#include "player.h"

character::character(std::unique_ptr<player>&& my_player) :
	mPlayer(std::move(my_player)),
	counter(RPSStat::NUETRAL),
	lvf_balance(0.f),
	role(Role::NUM_ROLES),
	synergy(0.f),
	farm(0.f),
	level(0.f),
	streak(0)
{
	kda[0] = 0;
	kda[1] = 0;
	kda[2] = 0;
}

character::~character()
{
}

short Counter(character* left, character* right)
{
	if ((left->GetRPS() + 1) % 3 == right->GetRPS())
		return -1;
	if ((right->GetRPS() + 1) % 3 == left->GetRPS())
		return 1;
	return 0;
}

const float character::GetSkill() const
{
	float tilt = mPlayer->GetTilt();
	float skill = (mPlayer->GetSkill() + mPlayer->GetRoleSkill(role));
	if (tilt < 0)
		return skill * (1 + tilt);
	else
		return skill + (1 - skill) * tilt;
}

const float character::GetPower()
{
	if (role == DISCONNECTED)
		return 0.f;
	float power = GetSkill();
	power *= 1 + synergy;
	power = 0.5f * std::max<float>(0.f, std::min<float>(2.f, power + (level * lvf_balance + farm * (1 - lvf_balance)) / 500.f));
	return power;
}

void character::LaneEarnings(float exp, float gold)
{
	level = std::min<float>(500.f, std::max<float>(0.f, level + exp));
	farm = std::min<float>(500.f, std::max<float>(0.f, farm + gold));
}

const float DEATH_TAX = 25.f;

void character::Kill(character* victim)
{
	++kda[0];
	streak = 1 + std::max<short>(0, streak);
	float modifier = 1 + (victim->GetStreak() * 0.05f) + 0.1f * (victim->GetLevel() - level);
	modifier = std::min<float>(3.f, std::max<float>(0.5f, modifier));
	LaneEarnings(DEATH_TAX * modifier, DEATH_TAX * modifier);
	
	mPlayer->UpdateTilt(MultiRand(0.01f, 0.1f, 3));

	victim->Death();
}
void character::Death()
{
	++kda[1];
	streak = std::min<short>(0, streak) - 1;
	float modifier = 1 + (streak * 0.05f);
	modifier = std::min<float>(3.f, std::max<float>(0.5f, modifier));
	LaneEarnings(-DEATH_TAX * modifier, -DEATH_TAX * modifier);

	mPlayer->UpdateTilt(MultiRand(-0.01f, -0.1f, 3));
}
void character::Assist(character* victim, size_t num_assists)
{
	++kda[2];
	streak = std::max<short>(0, streak);
	float modifier = 1 + 0.1f * (victim->GetLevel() - level);
	modifier = std::min<float>(3.f, std::max<float>(0.5f, modifier)) / (num_assists + 1.f);
	LaneEarnings(DEATH_TAX * modifier, DEATH_TAX * modifier);

	mPlayer->UpdateTilt(MultiRand(0.003f, 0.03f, 3));
}
