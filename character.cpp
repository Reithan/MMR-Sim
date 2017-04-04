#include "stdafx.h"
#include "character.h"


character::character(player* const my_player) :
	mPlayer(my_player),
	counter(RPSStat::NUETRAL),
	lvf_balance(0.f),
	role(Role::NUM_ROLES),
	synergy(0.f)
{
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
