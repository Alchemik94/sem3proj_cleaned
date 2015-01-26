#pragma once

#include "Champion.h"

namespace Game
{
	class ChampionController
	{
		protected:
			volatile bool* _paused;
			Champion* _controlledChampion;
		public:
			ChampionController(Champion* controlledChampion, volatile bool* paused);
			virtual ~ChampionController();
	};
}