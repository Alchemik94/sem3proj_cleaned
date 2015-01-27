#include "ChampionController.h"

namespace Game
{
	ChampionController::ChampionController(Champion* controlledChampion)
	{
		_controlledChampion = controlledChampion;
	}

	ChampionController::~ChampionController()
	{
		_controlledChampion = NULL;
	}
}