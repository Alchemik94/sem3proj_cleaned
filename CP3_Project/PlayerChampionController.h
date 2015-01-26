#pragma once

#include "ChampionController.h"
#include "KeyCatcher.h"
#include "Team.h"

namespace Game
{
	class PlayerChampionController :public ChampionController, public Application::KeyCatcher
	{
		protected:
			virtual void CatchedKeyHandler(Application::Keys key);
			Team* _enemyTeam;
		public:
			PlayerChampionController(Champion* controlledChampion, volatile bool* paused, Team* enemyTeam, Application::KeyCatcher* masterKeyCatcher);
	};
}