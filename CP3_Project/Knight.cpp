#ifndef _KNIGHT_DEFINITION
#define _KNIGHT_DEFINITION

#include "Knight.h"
#include "EnemiesFilter.h"

namespace Game
{
	Knight::Knight(ReadyPreset preset) :Champion(preset)
	{
	
	}

	Application::IFilter* Knight::CreateFilter()
	{
		return (new EnemiesFilter())->Append(new AliveEnemiesFilter())->Append(new LaneEnemiesFilter())->Append(new DistanceEnemiesFilter());
	}
}

#endif
