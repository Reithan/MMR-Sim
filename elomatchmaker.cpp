#include "stdafx.h"
#include "elomatchmaker.h"
#include "player.h"

elomatchmaker::elomatchmaker()
{
}


elomatchmaker::~elomatchmaker()
{
}

const float elomatchmaker::LogRating(const float rating) const
{
	return std::powf(10.f, rating / 400);
}

const float elomatchmaker::Expectation(const float first, const float second) const
{
	float Ri = LogRating(first);
	return Ri / (Ri + LogRating(second));
}

void elomatchmaker::ReportMatch(match* match_ended, const unsigned short winner)
{
	float team_avg[2] =
	{
		match_ended->GetTeam(0)->GetAvgRating(),
		match_ended->GetTeam(1)->GetAvgRating(),
	};

	for (size_t iteam = 0; iteam < 2; ++iteam)
	{
		for (size_t iplayer = 0; iplayer < 5; ++iplayer)
		{
			auto player_ptr = match_ended->GetTeam(iteam)->GetPlayer(iplayer).get();
			float expected = Expectation(player_ptr->GetRating(), team_avg[1 - iteam]);
			if (k * ((winner == iteam) - expected) < 0 && winner == iteam)
				std::cout << "WTF?!";
			player_ptr->UpdateRating(k * ((winner == iteam) - expected));
			player_ptr->UpdateTilt(0.1f * (2 * (winner == iteam) - 1));
		}
	}

	static short count = 0;
	if (++count == 100)
	{
		count = 0;
		std::cout << team_avg[0] << (winner == 0 ? "(W)" : "") << " v. "
			<< team_avg[1] << (winner == 1 ? "(W)" : "");
		if (std::abs(team_avg[0] - team_avg[1]) > 200.f && (team_avg[0] > team_avg[1]) != (winner == 0))
			std::cout << "!!! => ";
		else
			std::cout << " => ";

		std::cout << match_ended->GetTeam(0)->GetAvgRating() << " v. "
			<< match_ended->GetTeam(1)->GetAvgRating() << '\n';
	}
}
