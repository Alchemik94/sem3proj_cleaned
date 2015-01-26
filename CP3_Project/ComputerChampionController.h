#pragma once

#include "ChampionController.h"
#include "Team.h"
#include "Filter.h"
#include "Timer.h"
#include "ITimerParam.h"
#include <thread>

namespace Game
{
	class ComputerChampionController :public ChampionController, Application::ITimerParameter
	{
		private:
			Application::Timer* _timer;
			Team* _enemyTeam;
			Application::IFilter* _filter;
			unsigned int LowestSensiblePauseTime(Champion* champion);
			static void TakeTheAction(Application::ITimerParameter* param);
		public:
			ComputerChampionController(Champion* controlledChampion, volatile bool* paused, Team* enemyTeam);
			virtual void Start();
			virtual void Stop();
			virtual ~ComputerChampionController();
	};
}