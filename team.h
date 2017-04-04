#pragma once
#include <memory>

//forward dec
class character;

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

