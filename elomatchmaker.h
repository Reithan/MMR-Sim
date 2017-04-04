#pragma once
#include "matchmaker.h"
class elomatchmaker : public matchmaker
{
	const float k = 32;
public:
	elomatchmaker();
	virtual ~elomatchmaker();

	const float LogRating(const float rating) const;
	const float Expectation(const float first, const float second) const;

	virtual void ReportMatch(match* match_ended, const unsigned short winner);
};

