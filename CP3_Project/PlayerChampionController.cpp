#include "PlayerChampionController.h"
#include "SingleDataKeeper.h"

#define elif else if

namespace Game
{
	PlayerChampionController::PlayerChampionController(Champion* controlledChampion, volatile bool* paused, Team* enemyTeam, Application::KeyCatcher* masterKeyCatcher): ChampionController(controlledChampion, paused), Application::KeyCatcher(masterKeyCatcher)
	{
		_enemyTeam = enemyTeam;
	}

//TODO
	void PlayerChampionController::CatchedKeyHandler(Application::Keys key)
	{
		if (*_paused == true)
//Error throwing here
			;
		const Application::Keys
			UpKey = (const Application::Keys)Application::SingleDataKeeper::Instance()->GetInt("UpKey"),
			DownKey = (const Application::Keys)Application::SingleDataKeeper::Instance()->GetInt("DownKey"),
			LeftKey = (const Application::Keys)Application::SingleDataKeeper::Instance()->GetInt("LeftKey"),
			RightKey = (const Application::Keys)Application::SingleDataKeeper::Instance()->GetInt("RightKey"),
			AttackKey = (const Application::Keys)Application::SingleDataKeeper::Instance()->GetInt("AttackKey"),
			PauseKey = (const Application::Keys)Application::SingleDataKeeper::Instance()->GetInt("PauseKey");

		if (key == UpKey)
			_controlledChampion->Move(Direction::Up);
		elif(key == DownKey)
			_controlledChampion->Move(Direction::Down);
		elif(key == LeftKey)
			_controlledChampion->Move(Direction::Left);
		elif(key == RightKey)
			_controlledChampion->Move(Direction::Right);
		elif(key == AttackKey)
			_controlledChampion->Attack(_enemyTeam);
		elif(key == PauseKey)
			ReturnControl();
		else
		{ }
	}
}

#undef elif