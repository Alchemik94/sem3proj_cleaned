#ifndef _CHAMPIONFACTORY_DEFINITION
#define _CHAMPIONFACTORY_DEFINITION

#include "ChampionFactory.h"

namespace Game
{
	Champion* ChampionFactory::CreateChampion(ReadyPreset preset)
	{
		switch (preset)
		{
			case ReadyPreset::AIKnight:
			case ReadyPreset::PlayerKnight:
				return new Knight(preset);
			default:
				//throw an error
				break;
		}
	}
}

#endif