#pragma once

#include "Champion.h"

namespace Game
{
	class ChampionController
	{
		protected:
			Champion* _controlledChampion;
		public:
			ChampionController(Champion* controlledChampion);
			virtual ~ChampionController();
	};
}