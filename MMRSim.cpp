// MMRSim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "player.h"
#include "matchmaker.h"
#include "elomatchmaker.h"

int main()
{
	std::unique_ptr<matchmaker> mm_queue;
	// select MMR style (TODO)
	mm_queue.reset(new elomatchmaker());

	// generate or load players
	std::fstream player_db;
	player_db.open("PlayerData.dat", std::ios::binary | std::ios::in);
	if (player_db.is_open())
	{
		unsigned short count;
		player_db.read((char*)&count,sizeof(unsigned short));
		for (size_t iplayer = 0; iplayer < count; ++iplayer)
		{
			mm_queue->AccessAllPlayers().push_back(std::make_unique<player>());
			auto char_ptr = mm_queue->AccessAllPlayers().back().get();
			player_db.read((char*)char_ptr, sizeof(player));
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
					role_skills[iskill] = MultiRand(-5.f, 5.f, 2);
					skill_total += role_skills[iskill];
				}
				mm_queue->AccessAllPlayers().push_back(std::unique_ptr<player>(new player(MultiRand(0.f, 1.f, 2), role_skills, MultiRand(0.f, 1.f, 3), 0.0f)));
				auto char_ptr = mm_queue->AccessAllPlayers().back().get();
				player_db.write((char*)char_ptr, sizeof(player));
			}
			player_db.close();
		}
		else
			return 1;
	}
	// main loop
	std::vector<std::unique_ptr<match>> live_matches;
	while (true)
	{
		// add/remove players from queue
		while (mm_queue->NumQueued() < 100 || std::rand() % 10 == 0)
		{
			mm_queue->QueuePlayer();
		}
		if (rand() % 10 == 0)
			mm_queue->DropPlayer();
		// use matchmaker to attempt to form matches
		auto new_match = mm_queue->FormMatch();
		if(nullptr != new_match)
			live_matches.push_back(std::move(new_match));
		// run sim-step for currently running matches
		for (auto imatch = live_matches.begin(); imatch != live_matches.end(); )
		{
			if (false == (*imatch)->Update())
			{
				mm_queue->ReportMatch(imatch->get(), (*imatch)->Winner());

				// re-queue players after match based on random + tilt
				mm_queue->ReQueue(imatch->get());
				imatch = live_matches.erase(imatch);
			}
			else
				++imatch;
		}
	}
	// print stats on MMR accuracy and match stats
    return 0;
}

