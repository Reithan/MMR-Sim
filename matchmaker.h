#pragma once
#include "match.h"

class matchmaker
{
public:
	typedef std::list<std::unique_ptr<player>> player_set;
private:
	player_set all_players;
	player_set queued_players;
	std::unique_ptr<match> pending_match;
public:
	matchmaker();
	virtual ~matchmaker();

	virtual void ReportMatch(match* match_ended, const unsigned short winner) = 0;

	void ReQueue(match* match_ptr);
	std::unique_ptr<match> FormMatch();
	bool QueuePlayer(std::unique_ptr<player>&& new_player = nullptr);
	bool DropPlayer();

	size_t NumQueued() { return queued_players.size(); }
	player_set& AccessAllPlayers() { return all_players; }
};

