#include "ChampionController.h"

namespace Game
{
	ChampionController::ChampionController(Champion* controlledChampion, volatile bool* paused)
	{
		_controlledChampion = controlledChampion;
		_paused = paused;
	}

	ChampionController::~ChampionController()
	{
		_controlledChampion = NULL;
	}
}