#include "stdafx.h"
#include "elomatchmaker.h"


elomatchmaker::elomatchmaker()
{
}


elomatchmaker::~elomatchmaker()
{
}

std::unique_ptr<match> elomatchmaker::FormMatch()
{
	return nullptr;
}

bool elomatchmaker::QueuePlayer(const player* new_player)
{
	return false;
}

bool elomatchmaker::DropPlayer(const player* del_player)
{
	return false;
}

void elomatchmaker::ReportMatch(const match* match_ended, const unsigned short winner)
{

}
