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
		for (size_t iplayer = 0; iplayer < 10; ++iplayer)
		{
			auto player_ptr = match_ended->GetPlayer(iteam, iplayer % 5);
			float expected = Expectation(team_avg[iteam], team_avg[1 - iteam]);
			player_ptr->UpdateRating(k * ((winner == iteam) - expected));
		}
	}
}
