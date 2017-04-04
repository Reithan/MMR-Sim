#pragma once
#include "match.h"

class matchmaker
{
	std::unordered_set<player*> queued_players;
	std::unique_ptr<match> pending_match;
public:
	matchmaker();
	virtual ~matchmaker();

	virtual void ReportMatch(match* match_ended, const unsigned short winner) = 0;

	void ReQueue(match* match_ptr);
	std::unique_ptr<match> FormMatch();
	bool QueuePlayer(player* const new_player);
	bool DropPlayer(player* const del_player = nullptr);

	size_t NumQueued() { return queued_players.size(); }
};

