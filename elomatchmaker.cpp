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
	for (size_t iteam = 0; iteam < 2; ++iteam)
	{
		for (size_t iplayer = 0; iplayer < 10; ++iplayer)
		{
			auto player_ptr = match_ended->GetPlayer(iteam, iplayer % 5);
			float expected = Expectation(match_ended->GetTeam(iteam)->GetAvgRating(), match_ended->GetTeam(1 - iteam)->GetAvgRating());
			player_ptr->UpdateRating(k * ((winner == iteam) - expected));
		}
	}
}
