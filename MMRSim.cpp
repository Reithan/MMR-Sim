// MMRSim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "player.h"
#include "matchmaker.h"
#include "elomatchmaker.h"

int main()
{
	// generate or load players
	std::vector<std::unique_ptr<player>> all_players;
	std::fstream player_db;
	player_db.open("PlayerData.dat", std::ios::binary | std::ios::in);
	if (player_db.is_open())
	{
		unsigned short count;
		player_db.read((char*)&count,sizeof(unsigned short));
		for (size_t iplayer = 0; iplayer < count; ++iplayer)
		{
			all_players.emplace_back(new player);
			player_db.read((char*)all_players.back().get(), sizeof(player));
		}
		player_db.close();
	}
	else
	{
		player_db.open("PlayerData.dat", std::ios::binary | std::ios::out);
		if (player_db.is_open())
		{
			unsigned short count = 1000;
			player_db.write((char*)&count, sizeof(unsigned short));
			for (size_t iplayer = 0; iplayer < 1000; ++iplayer)
			{
				std::vector<float> role_skills(Role::NUM_ROLES);
				float skill_total = 0.f;
				for (size_t iskill = 0; iskill < Role::NUM_ROLES; ++iskill)
				{
					role_skills[iskill] = (rand() % 100 - 50) / 10.f - skill_total;
					skill_total += role_skills[iskill];
				}
				all_players.push_back(std::unique_ptr<player>(new player(rand() % 1000 / 10.f, role_skills, rand() % 1000 / 10.f, 0.0f)));
				player_db.write((char*)all_players.back().get(), sizeof(player));
			}
			player_db.close();
		}
		else
			return 1;
	}
	// main loop
	std::unique_ptr<matchmaker> mm_queue;
	std::vector<std::unique_ptr<match>> live_matches;
	// select MMR style (TODO)
	mm_queue.reset(new elomatchmaker());
	while (true)
	{
		// add/remove players from queue
		while (mm_queue->NumQueued() < 100 || std::rand() % 10 == 0)
		{
			// TODO: Can re-queue duplicate players
			mm_queue->QueuePlayer(all_players[std::rand() % all_players.size()].get());
		}
		if (rand() % 10 == 0)
			mm_queue->DropPlayer();
		// use matchmaker to attempt to form matches
		auto new_match = mm_queue->FormMatch();
		if(nullptr != new_match)
			live_matches.push_back(std::move(new_match));
		// run sim-step for currently running matches
		for (auto imatch = live_matches.begin(); imatch != live_matches.end(); ++imatch)
		{
			if (false == (*imatch)->Update())
			{
				mm_queue->ReportMatch(imatch->get(), (*imatch)->Winner());

				// re-queue players after match based on random + tilt
				mm_queue->ReQueue(imatch->get());
			}
		}
	}
	// print stats on MMR accuracy and match stats
    return 0;
}

