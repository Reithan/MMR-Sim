#pragma once
#include "roles.h"
class player
{
	float skill;
	float role_skills[Role::NUM_ROLES];
	float irritability;
	float tilt;
	float rating;
public:
	player();
	player(float skill, const std::vector<float>& role_skills, float irritability, float tilt);
	~player();

	const float GetTilt() const { return tilt; }
	const float GetRating() const { return rating; }

	void UpdateRating(float delta) { rating += delta; }
	void UpdateTilt(float raw_delta) { tilt += raw_delta * irritability; }
};

