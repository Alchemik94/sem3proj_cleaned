#ifndef _GAMEHOLDER_DEFINITION
#define _GAMEHOLDER_DEFINITION

#include "GameHolder.h"
#include "Application.h"
#include "Timer.h"
#include "PlayerChampionController.h"
#include "ComputerChampionController.h"
#include "Knight.h"

namespace Game
{
//TODO
//Get rid of hardcode
	GameHolder::GameHolder()
	{
		_paused = true;
		Show();
		_team1 = new Team();
		_team1->push_back(new Knight(PlayerKnight));
		_team2 = new AutogeneratingTeam(5, ReadyPreset::AIKnight);
		_team1Controllers = std::vector<ChampionController*>{new PlayerChampionController(*_team1->begin(),&_paused,_team2,this), };
		for (auto championPtr = _team2->begin(); championPtr!=_team2->end(); ++championPtr)
			_team2Controllers.push_back(new ComputerChampionController(*championPtr,&_paused,_team1));
		_roundsNumber = 1;
		_currentRound = 0;
		_menu = new PauseMenu(this,this);
		_running = true;
		_runner = new std::thread(RoundsRunner, this);
		PausedGame();
	}

//TODO
//Get rid of hardcode
	GameHolder::GameHolder(int numberOfRounds)
	{
		_paused = true;
		Show();
		_team1 = new Team();
		_team1->push_back(new Knight(PlayerKnight));
		_team2 = new AutogeneratingTeam(5, ReadyPreset::AIKnight);
		_team1Controllers = std::vector<ChampionController*>{new PlayerChampionController(*_team1->begin(), &_paused, _team2,this), };
		for (auto championPtr = _team2->begin(); championPtr != _team2->end(); ++championPtr)
			_team2Controllers.push_back(new ComputerChampionController(*championPtr, &_paused, _team1));
		_roundsNumber = numberOfRounds;
		_currentRound = 0;
		_menu = new PauseMenu(this,this);
		_running = true;
		_runner = new std::thread(RoundsRunner, this);
		PausedGame();
	}

	GameHolder::~GameHolder()
	{
		Exit();
		Hide();
		if (_runner->joinable())
			_runner->join();
		if (_team1 != NULL)
			delete _team1;
		if (_team2 != NULL)
			delete _team2;
	}

	void GameHolder::CatchedKeyHandler(Application::Keys key)
	{
		if (_running)
			throw "GameHolder cannot catch a key!\n";
	}

	//TODO
	//Needs changes for various types of enemies
	void GameHolder::NewRound(int numberOfEnemies)
	{
		LOCK_APPLICATION_VARIABLES(Application::EmptyTimer::Instance());
//TODO
//error throwing
		if (_currentRound++ >= _roundsNumber)
		{
			UNLOCK_APPLICATION_VARIABLES;
			return; //throw an error
		}
		UNLOCK_APPLICATION_VARIABLES;
		while (_team2->size() > 0 && _running) std::this_thread::yield();
		if (_running)
		{
			LOCK_APPLICATION_VARIABLES(Application::EmptyTimer::Instance());
			delete _team2;
			IGameDisplayer::NewRound();
			_team2 = new AutogeneratingTeam(numberOfEnemies, AIKnight);
			_team2Controllers.clear();
			for (auto championPtr = _team2->begin(); championPtr != _team2->end(); ++championPtr)
				_team2Controllers.push_back(new ComputerChampionController(*championPtr, &_paused, _team1));
			UNLOCK_APPLICATION_VARIABLES;
		}
		else
		{
			//do nothing, just exit
		}
	}

	inline int GameHolder::CurrentRound()
	{
		return _currentRound;
	}

	inline int GameHolder::RoundsNumber()
	{
		return _roundsNumber;
	}

	void GameHolder::EnteredGame()
	{
		_menu->Hide();
		GiveControl(GetPlayerController());
		Unpause();
	}

	void GameHolder::PausedGame()
	{
		Pause();
		_menu->Show();
		GiveControl(_menu);
	}

	void GameHolder::Exit()
	{
		Pause();
		if (_menu!=NULL)
			_menu->Hide();
		_running = false;
		_currentRound = _roundsNumber;
		if (_runner->joinable())
			_runner->join();
		if (_team1 != NULL)
		{
			delete _team1;
			_team1 = NULL;
		}
		if (_team2 != NULL)
		{
			delete _team2;
			_team2 = NULL;
		}
		if (_menu != NULL)
		{
			delete _menu;
			_menu = NULL;
		}
	}

	inline void GameHolder::Pause()
	{
		_paused = true;
	}

	inline void GameHolder::Unpause()
	{
		_paused = false;
	}

	inline bool GameHolder::Running()
	{
		return _running;
	}

	void GameHolder::RoundsRunner(GameHolder* holder)
	{
		while (holder->_running && holder->_currentRound < holder->_roundsNumber)
			holder->NewRound(5);
		LOCK_APPLICATION_VARIABLES(Application::EmptyTimer::Instance());
		holder->_running = false;
		UNLOCK_APPLICATION_VARIABLES;
	}

//TODO
//More players case
	Application::KeyCatcher* GameHolder::GetPlayerController()
	{
		return dynamic_cast<Application::KeyCatcher*>(*_team1Controllers.begin());
	}
}

#endif