#pragma once
#include "matchmaker.h"
class elomatchmaker :
	public matchmaker
{
public:
	elomatchmaker();
	virtual ~elomatchmaker();

	virtual std::unique_ptr<match> FormMatch();
	virtual bool QueuePlayer(const player* new_player);
	virtual bool DropPlayer(const player* del_player);
	virtual void ReportMatch(const match* match_ended, const unsigned short winner);
};

