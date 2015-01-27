#ifndef _GAMEHOLDER_DECLARATION
#define _GAMEHOLDER_DECLARATION

#include "IGameDisplayer.h"
#include "Champion.h"
#include <vector>
#include "Team.h"
#include "KeyCatcher.h"
#include "PauseMenu.h"
#include "ChampionController.h"
#include <thread>
#include "EventHandler.h"
#include "EventHandler.h"

namespace Game
{
	class GameHolder: public Display::IGameDisplayer, public Application::KeyCatcher, Application::Object
	{
		private:
			Team* _team1;
			Team* _team2;
			int _roundsNumber;
			int _currentRound;
			bool _paused;
			static void RoundsChanger(Application::Object* sender, Application::EventArgs*, Application::Object* instance);
			//changes game state
			void Pause();
			//changes game state
			void Unpause();
			Application::KeyCatcher* GetPlayerController();
			PauseMenu* _menu;
			bool _running;
			std::vector<ChampionController*> _team1Controllers;
			std::vector<ChampionController*> _team2Controllers;
			virtual void CatchedKeyHandler(Application::Keys key);
		public:
			Application::EventHandler FrameElapsed;
			GameHolder();
			GameHolder(int numberOfRounds);
			virtual ~GameHolder();
			virtual void NewRound(int numberOfEnemies);
			virtual int RoundsNumber();
			virtual bool Running();
			virtual int CurrentRound();
			//after changing game state displays game
			virtual void EnteredGame();
			//after changing game state displays menu
			virtual void PausedGame();
			virtual void Exit();
	};
}

#endif