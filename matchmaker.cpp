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
		auto player_ptr = std::move(match_ptr->GetPlayer(iplayer / 5, iplayer % 5));
		if (nullptr != player_ptr && rand() % 3 != 0 && player_ptr->GetTilt() > -0.75f)
			QueuePlayer(std::move(player_ptr));
		else
			all_players.push_back(std::move(player_ptr));
	}
}

std::unique_ptr<match> matchmaker::FormMatch()
{
	static const float INITIAL_WINDOW = 10.f, WINDOW_UPDATE = 5.f;
	if (queued_players.empty())
		return nullptr;

	pending_match.reset(new match());
	unsigned short num_players[2] = { 1, 0 };
	auto iplayer = queued_players.begin();
	if (pending_match->GetTeam(0)->AddPlayer(std::move(*iplayer)))
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
	float window = INITIAL_WINDOW;

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
				window += WINDOW_UPDATE;
				iplayer = queued_players.begin();
			}
			float avg_ratings[2] =
			{
				pending_match->GetTeam(0)->GetAvgRating(),
				pending_match->GetTeam(1)->GetAvgRating(),
			};
			float avg_rating = avg_ratings[0];
			if (!isnan(avg_ratings[1]))
				avg_rating = (avg_rating + avg_ratings[1]) * 0.5f;
			if (abs((*iplayer)->GetRating() - avg_rating) <= window &&
				(isnan(avg_ratings[next_team]) || abs((*iplayer)->GetRating() - avg_ratings[next_team]) <= window) &&
				pending_match->GetTeam(next_team)->AddPlayer(std::move(*iplayer)))
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

bool matchmaker::QueuePlayer(std::unique_ptr<player>&& new_player)
{
	if (new_player != nullptr)
	{
		queued_players.push_back(std::move(new_player));
		all_players.remove(new_player);
		return true;
	}

	for (auto iplayer = all_players.begin(); iplayer != all_players.end(); ++iplayer)
	{
		(*iplayer)->UpdateTilt(0.25f);
		if ((*iplayer)->GetTilt() > 0.25f)
		{
			queued_players.push_back(std::move((*iplayer)));
			all_players.erase(iplayer);
			return true;
		}
	}
	return false;
}

bool matchmaker::DropPlayer()
{
	if (queued_players.empty())
		return false;

	auto i = queued_players.begin();
	for (; rand() % 10 == 0; )
	{
		if (++i == queued_players.end())
			i = queued_players.begin();
	}
	all_players.push_back(std::move(*i));
	queued_players.erase(i);
	return true;
}
