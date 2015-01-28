#pragma once

#include "Champion.h"
#include "Knight.h"
#include "GameEnums.h"

namespace Game
{
	class ChampionFactory
	{
		private:
			ChampionFactory(){}
		public:
			static Champion* CreateChampion(ReadyPreset preset);
	};
}