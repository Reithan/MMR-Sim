#pragma once
#include "match.h"

class matchmaker
{
	std::vector<player*> queued_players;
	std::vector<std::unique_ptr<match>> pending_matches;
public:
	matchmaker();
	virtual ~matchmaker();

	virtual std::unique_ptr<match> FormMatch()		= 0;
	virtual bool QueuePlayer(const player* new_player)	= 0;
	virtual bool DropPlayer(const player* del_player = nullptr)		= 0;
	virtual void ReportMatch(const match* match_ended, const unsigned short winner) = 0;

	size_t NumQueued() { return queued_players.size(); }
};

