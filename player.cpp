#include "stdafx.h"
#include "player.h"


player::player(float skill, std::vector<float> role_skills, float irritability, float tilt) :
	skill(skill),
	irritability(irritability),
	tilt(tilt)
{
	for (size_t irole = 0; irole < Role::NUM_ROLES; ++irole)
	{
		this->role_skills[irole] = role_skills[irole];
	}
}


player::~player()
{
}
