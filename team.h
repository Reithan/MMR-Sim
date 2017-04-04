#pragma once
#include "character.h"

class team
{
	float CC;
	float Tank;
	float Burst;
	float DPS;

	std::unique_ptr<character> character[5];
public:
	team();
	~team();
};

