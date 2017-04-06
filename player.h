#pragma once
#include "roles.h"
class player
{
	float skill;
	float role_skills[Role::NUM_ROLES];
	float irritability;
	float tilt;
	float rating;
	unsigned short games_played;

public:
	player();
	player(float skill, const std::vector<float>& role_skills, float irritability, float tilt);
	~player();

	const float GetTilt() const { return tilt; }
	const float GetRating() const { return rating; }
	const float GetSkill() const { return skill; }
	const float GetRoleSkill(Role role) const { return role_skills[role]; }
	const unsigned short GetGames() const { return games_played; }
	const float GetIrritibility() const { return irritability; }

	void UpdateRating(float delta) { rating = std::max<float>(0.f, rating + delta); ++games_played; }
	void UpdateTilt(float raw_delta);
};

