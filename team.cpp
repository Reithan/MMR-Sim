#include "stdafx.h"
#include "team.h"
#include "player.h"

team::team()
{
}


team::~team()
{
}

bool team::AddPlayer(player* const new_player)
{
	for (size_t i = 0; i < 5; ++i)
	{
		if (nullptr == characters[i])
		{
			characters[i].reset(new character(new_player));
			return true;
		}
	}
	return false;
}

const float team::GetAvgRating() const
{
	float total = 0;
	unsigned short num_players = 0;
	for (size_t i = 0; i < 5; ++i)
	{
		if (characters[i] != nullptr)
		{
			total += characters[i]->GetPlayer()->GetRating();
			++num_players;
		}
	}
	return total / num_players;
}

void team::SortByRole()
{
	std::sort(&characters[0], &characters[5], [](std::unique_ptr<character>& left, std::unique_ptr<character>& right)->bool
		{
			return left->GetRole() < right->GetRole();
		}
	);
}
