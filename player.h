#pragma once
#include "roles.h"
class player
{
	float skill;
	float role_skills[Role::NUM_ROLES];
	float irritability;
	float tilt;
public:
	player();
	~player();
};

