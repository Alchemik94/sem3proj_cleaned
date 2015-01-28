#include "GameHolder.h"
#include "Application.h"
#include "PlayerChampionController.h"
#include "ComputerChampionController.h"
#include "Knight.h"

namespace Game
{
	GameHolder::GameHolder(int numberOfRounds) : IGameDisplayer(Application::SingleDataKeeper::Instance()->GetString("BackgroundImagePath"))
	{
		Show();
		_team1 = new Team();
		_team1->push_back(new Knight(PlayerKnight));
		_team2 = new AutogeneratingTeam(5, ReadyPreset::AIKnight);
		_team1Controllers = std::vector<ChampionController*>{new PlayerChampionController(*_team1->begin(), _team2,this), };
		for (auto championPtr = _team2->begin(); championPtr != _team2->end(); ++championPtr)
			_team2Controllers.push_back(new ComputerChampionController(*championPtr, _team1));
		_roundsNumber = numberOfRounds;
		_currentRound = 0;
		_menu = new PauseMenu(this,this);
		_running = true;
		FrameElapsed += std::make_pair(this,RoundsChanger);
		FrameElapsed += _team1->FrameElapsed;
		FrameElapsed += _team2->FrameElapsed;
		PausedGame();
	}

	GameHolder::~GameHolder()
	{
		Exit();
		Hide();
		if (_team1 != NULL)
		{
			delete _team1;
			_team1 = NULL;
		}

		for (auto championControllers : _team1Controllers)
			delete championControllers;
		_team1Controllers.clear();

		if (_team2 != NULL)
		{
			delete _team2;
			_team2 = NULL;
		}

		for (auto championControllers : _team2Controllers)
			delete championControllers;
		_team2Controllers.clear();

		if (_menu != NULL)
		{
			delete _menu;
			_menu = NULL;
		}
	}

	void GameHolder::CatchedKeyHandler(Application::Keys key)
	{
		if (_running)
			throw "GameHolder cannot catch a key!\n";
	}

	//NOTE: Needs changes in the case of various types of enemies
	void GameHolder::NewRound(int numberOfEnemies)
	{
		if (_currentRound++ >= _roundsNumber)
		{
			throw "Rounds number exceeded.";
		}
		if (_running)
		{
			//cleaning
			delete _team2;
			for (auto championController : _team2Controllers)
				delete championController;
			_team2Controllers.clear();

			//GUI effect
			IGameDisplayer::NewRound();

			//new round enemy team generating
			_team2 = new AutogeneratingTeam(numberOfEnemies, AIKnight);
			for (auto championPtr = _team2->begin(); championPtr != _team2->end(); ++championPtr)
				_team2Controllers.push_back(new ComputerChampionController(*championPtr, _team1));
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
		_running = false;
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

	void GameHolder::RoundsChanger(Application::Object* sender, Application::EventArgs*, Application::Object* instance)
	{
		GameHolder* holder = dynamic_cast<GameHolder*>(instance);
		if (holder->_running && holder->_currentRound < holder->_roundsNumber)
		{
			if (holder->_team2->size() == 0)
				holder->NewRound(5);
		}
		else
			holder->Exit();
	}

	//NOTE: At more players case it may be greatly expanded
	Application::KeyCatcher* GameHolder::GetPlayerController()
	{
		return dynamic_cast<Application::KeyCatcher*>(*_team1Controllers.begin());
	}
}