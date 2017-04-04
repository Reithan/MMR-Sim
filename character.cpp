#include "stdafx.h"
#include "character.h"


character::character(const player* my_player, const RPSStat counter, const float lvf_balance, const Role role, const float synergy) :
	mPlayer(my_player),
	counter(counter),
	lvf_balance(lvf_balance),
	role(role),
	synergy(synergy)
{
}

character::~character()
{
}
