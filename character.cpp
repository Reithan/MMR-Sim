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
