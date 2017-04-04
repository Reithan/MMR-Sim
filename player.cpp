#include "stdafx.h"
#include "player.h"


player::player() :
	skill(0.f),
	irritability(0.f),
	tilt(0.f),
	rating(0.f)
{
	for (size_t irole = 0; irole < Role::NUM_ROLES; ++irole)
	{
		this->role_skills[irole] = 0.f;
	}
}

player::player(float skill, const std::vector<float>& role_skills, float irritability, float tilt) :
	skill(skill),
	irritability(irritability),
	tilt(tilt),
	rating(0.f)
{
	for (size_t irole = 0; irole < Role::NUM_ROLES; ++irole)
	{
		this->role_skills[irole] = role_skills[irole];
	}
}

player::~player()
{
}
