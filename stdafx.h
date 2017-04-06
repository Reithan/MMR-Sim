// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <tchar.h>

#include <vector>
#include <list>
#include <utility>
#include <memory>
#include <algorithm>
#include <random>
#include <math.h>

#include "player.h"

// TODO: reference additional headers your program requires here
float MultiRand(float min, float max, size_t multi = 1);

extern std::unique_ptr<player> null_player;
