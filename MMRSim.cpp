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
		player_db.close();
	}
	else
	{
		player_db.open("PlayerData.dat", std::ios::binary | std::ios::out);
		if (player_db.is_open())
		{
			for (size_t iplayer = 0; iplayer < 1000; ++iplayer)
			{
				all_players.push_back(std::unique_ptr<player>(new player(0.0f, { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f)));
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
			mm_queue->QueuePlayer(all_players[std::rand() % all_players.size()].get());
		}
		if (rand() % 10 == 0)
			mm_queue->DropPlayer();
		// use matchmaker to attempt to form matches
		live_matches.push_back(mm_queue->FormMatch());
		// run sim-step for currently running matches
		for (auto imatch = live_matches.begin(); imatch != live_matches.end(); ++imatch)
		{
			if (false == (*imatch)->Update())
			{
				mm_queue->ReportMatch(imatch->get(), (*imatch)->Winner());

				// re-queue players after match based on random + tilt
				for (size_t iplayer = 0; iplayer < 10; ++iplayer)
				{
					auto player_ptr = (*imatch)->GetPlayer(iplayer / 5, iplayer % 5);
					if(rand()%3 != 0 && player_ptr->GetTilt() < 0.75f)
						mm_queue->QueuePlayer(player_ptr);
				}
			}
		}
	}
	// print stats on MMR accuracy and match stats
    return 0;
}

