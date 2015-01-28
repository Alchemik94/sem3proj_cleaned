#ifndef _CHAMPIONFACTORY_DEFINITION
#define _CHAMPIONFACTORY_DEFINITION

#include "ChampionFactory.h"

namespace Game
{
	//NOTE: In the case of more champion types it would greatly expand
	Champion* ChampionFactory::CreateChampion(ReadyPreset preset)
	{
		switch (preset)
		{
			case ReadyPreset::AIKnight:
			case ReadyPreset::PlayerKnight:
				return new Knight(preset);
			default:
				throw "Unknown champion preset.";
				break;
		}
	}
}

#endif