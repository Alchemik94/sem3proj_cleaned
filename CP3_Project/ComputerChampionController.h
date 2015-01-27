#pragma once

#include "ChampionController.h"
#include "Team.h"
#include "Filter.h"
#include <thread>
#include "EventHandler.h"

namespace Game
{
	class ComputerChampionController :public ChampionController, public Application::Object
	{
		private:
			Team* _enemyTeam;
			Application::IFilter* _filter;
			static void TakeTheAction(Application::Object* sender, Application::EventArgs* e, Application::Object* instance);
		public:
			Application::EventHandler FrameElapsed;
			ComputerChampionController(Champion* controlledChampion, Team* enemyTeam);
			virtual ~ComputerChampionController();
	};
}