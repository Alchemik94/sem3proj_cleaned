#ifndef _GAMEHOLDER_DECLARATION
#define _GAMEHOLDER_DECLARATION

#include "IGameDisplayer.h"
#include "Champion.h"
#include <vector>
#include "Team.h"
#include "KeyCatcher.h"
#include "Timer.h"
#include "ITimerParam.h"
#include "PauseMenu.h"
#include "ChampionController.h"
#include <thread>

namespace Game
{
	class GameHolder: public Display::IGameDisplayer, public Application::KeyCatcher, private Application::ITimerParameter
	{
		private:
			Team* _team1;
			Team* _team2;
			int _roundsNumber;
			int _currentRound;
			volatile bool _paused;
			static void RoundsRunner(GameHolder* holder);
			void Pause();
			void Unpause();
			Application::KeyCatcher* GetPlayerController();
			PauseMenu* _menu;
			volatile bool _running;
			std::thread* _runner;
			std::vector<ChampionController*> _team1Controllers;
			std::vector<ChampionController*> _team2Controllers;
			virtual void CatchedKeyHandler(Application::Keys key);
		public:
			GameHolder();
			GameHolder(int numberOfRounds);
			virtual ~GameHolder();
			virtual void NewRound(int numberOfEnemies);
			virtual int RoundsNumber();
			virtual bool Running();
			virtual int CurrentRound();
			virtual void EnteredGame();
			virtual void PausedGame();
			virtual void Exit();
	};
}

#endif