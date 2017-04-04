#pragma once
#include "roles.h"
class player
{
	float skill;
	float role_skills[Role::NUM_ROLES];
	float irritability;
	float tilt;
public:
	player(float skill, std::vector<float> role_skills, float irritability, float tilt);
	~player();

	const float GetTilt() const { return tilt; }
};

