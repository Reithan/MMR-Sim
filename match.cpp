#include "stdafx.h"
#include "match.h"


match::match()
{
}


match::~match()
{
}

bool match::Update()
{
	switch (current_phase)
	{
	case match::DRAFT:
		break;
	case match::EARLY:
		break;
	case match::MID:
		break;
	case match::LATE:
		break;
	case match::NUM_PHASES:
		break;
	default:
		break;
	}
}