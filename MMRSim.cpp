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
					role_skills[iskill] = MultiRand(-0.05f, 0.05f, 2);
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
	const size_t NUM_MATCHES = 5000;
	size_t matches = 0;
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
		std::unique_ptr<match> new_match;
		if(matches < NUM_MATCHES)
			while(new_match == nullptr)
				new_match = mm_queue->FormMatch();
		++matches;

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

		if (matches >= NUM_MATCHES && live_matches.empty())
			break;
	}
	
	// Shut Down Server :P
	for (auto igame = live_matches.begin(); igame != live_matches.end(); ++igame)
		mm_queue->ReQueue(igame->get());
	while (mm_queue->DropPlayer());

	// print stats on MMR accuracy and match stats
	mm_queue->AccessAllPlayers().sort([](std::unique_ptr<player>& left, std::unique_ptr<player>& right)->bool{
		return left->GetRating() < right->GetRating();
	});

	auto ibegin = mm_queue->AccessAllPlayers().begin();
	auto iback = mm_queue->AccessAllPlayers().rbegin();
	float max_MMR = (*iback)->GetRating();
	float min_MMR = (*ibegin)->GetRating();

	auto imedian = ibegin;
	for (size_t i = 0; i < 500; i++)
		++imedian;
	float median_MMR = (*imedian)->GetRating();

	float low_MMR = 0.5f * (median_MMR + min_MMR);
	float high_MMR = 0.5f * (median_MMR + max_MMR);

	size_t players [3] = { 0  , };
	size_t games   [3] = { 0  , };
	float  skill   [3] = { 0.f, };
	float  irrit   [3] = { 0.f, };
	float  tilt    [3] = { 0.f, };

	for (auto iplayer = mm_queue->AccessAllPlayers().begin();
		iplayer != mm_queue->AccessAllPlayers().end();
		++iplayer)
	{
		float MMR = (*iplayer)->GetRating();
		if (MMR > high_MMR)
		{
			++players[2];
			  skill  [2]+= (*iplayer)->GetSkill();
			  games  [2]+= (*iplayer)->GetGames();
			  irrit  [2]+= (*iplayer)->GetIrritibility();
			  tilt   [2]+= (*iplayer)->GetTilt();
		}
		else if (MMR < low_MMR)
		{
			++players[0];
			  skill  [0]+= (*iplayer)->GetSkill();
			  games  [0]+= (*iplayer)->GetGames();
			  irrit  [0]+= (*iplayer)->GetIrritibility();
			  tilt   [0]+= (*iplayer)->GetTilt();
		}
		else
		{
			++players[1];
			  skill  [1]+= (*iplayer)->GetSkill();
			  games  [1]+= (*iplayer)->GetGames();
			  irrit  [1]+= (*iplayer)->GetIrritibility();
			  tilt   [1]+= (*iplayer)->GetTilt();
		}
	}

	for (size_t i = 0; i < 3; i++)
	{
		skill [i] /= players[i];
		games [i] /= players[i];
		irrit [i] /= players[i];
		tilt  [i] /= players[i];
	}

	std::cout << "\n\n";
	std::cout << "High MMR = " << high_MMR << "\tNum Players = " << players[2]
		<< "\n\tAvg Skill = " << skill[2] << "\tAvg Games = " << games[2]
		<< "\n\tAvg Irritibility = " << irrit[2] << "\tAvg Tilt = " << tilt[2] << '\n';
	std::cout << "Average MMR = " << median_MMR << "\tNum Players = " << players[1]
		<< "\n\tAvg Skill = " << skill[1] << "\tAvg Games = " << games[1]
		<< "\n\tAvg Irritibility = " << irrit[1] << "\tAvg Tilt = " << tilt[1] << '\n';
	std::cout << "Low MMR = " << low_MMR << "\tNum Players = " << players[0]
		<< "\n\tAvg Skill = " << skill[0] << "\tAvg Games = " << games[0]
		<< "\n\tAvg Irritibility = " << irrit[0] << "\tAvg Tilt = " << tilt[0] << '\n';
	std::getchar();
	return 0;
}

