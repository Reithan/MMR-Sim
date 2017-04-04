// MMRSim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "player.h"
#include "matchmaker.h"

int main()
{
	// generate or load players
	std::vector<std::unique_ptr<player>> all_players;
	// main loop
	matchmaker queue;
	std::vector<std::unique_ptr<match>> live_matches;
	while (true)
	{
		// add/remove players from queue
		// use matchmaker to attempt to form matches
		// run sim-step for currently running matches
		for (auto imatch = live_matches.begin(); imatch != live_matches.end(); ++imatch)
		{

		}
	}
	// print stats on MMR accuracy and match stats
    return 0;
}

