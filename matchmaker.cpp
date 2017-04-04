#include "stdafx.h"
#include "matchmaker.h"
#include "player.h"

matchmaker::matchmaker()
{
}


matchmaker::~matchmaker()
{
}

void matchmaker::ReQueue(match* match_ptr)
{
	for (size_t iplayer = 0; iplayer < 10; ++iplayer)
	{
		auto player_ptr = match_ptr->GetPlayer(iplayer / 5, iplayer % 5);
		if (nullptr != player_ptr && rand() % 3 != 0 && player_ptr->GetTilt() > -0.75f)
			QueuePlayer(player_ptr);
	}
}

std::unique_ptr<match> matchmaker::FormMatch()
{
	if (queued_players.empty())
		return nullptr;

	pending_match.reset(new match());
	unsigned short num_players[2] = { 1, 0 };
	auto iplayer = queued_players.begin();
	if (pending_match->GetTeam(0)->AddPlayer(*iplayer))
	{
		iplayer = queued_players.erase(iplayer);
		if (iplayer == queued_players.end())
			iplayer = queued_players.begin();
	}
	else
	{
		ReQueue(pending_match.get());
		return nullptr;
	}

	size_t loop = 0;
	float window = 100.f;

	for (; num_players[0] + num_players[1] < 10;)
	{
		size_t next_team = 1;
		if (num_players[0] == num_players[1])
			next_team = 0;

		while (true)
		{
			if (iplayer == queued_players.end())
			{
				++loop;
				if (loop > 10)
				{
					ReQueue(pending_match.get());
					return nullptr;
				}
				window += 50.f;
				iplayer = queued_players.begin();
			}
			float avg_rating = pending_match->GetTeam(next_team)->GetAvgRating();
			if ((abs((*iplayer)->GetRating() - avg_rating) <= window ||
					isnan(avg_rating)) &&
				pending_match->GetTeam(next_team)->AddPlayer(*iplayer))
			{
				++num_players[next_team];
				next_team = 1 - next_team;
				iplayer = queued_players.erase(iplayer);
				break;
			}
			++iplayer;
		}
	}
	return std::move(pending_match);
}

bool matchmaker::QueuePlayer(player* const new_player)
{
	return queued_players.insert(new_player).second;
}

bool matchmaker::DropPlayer(player* const del_player)
{
	if (queued_players.empty())
		return false;

	if (nullptr != del_player)
	{
		if (1 == queued_players.erase(del_player))
			return true;
	}
	else
	{
		auto i = queued_players.begin();
		for (; rand() % 100 == 0; ++i)
		{
			if (i == queued_players.end())
				i = queued_players.begin();
		}
		queued_players.erase(i);
		return true;
	}
	return false;
}
