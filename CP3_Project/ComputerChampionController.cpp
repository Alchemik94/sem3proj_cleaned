#include "ComputerChampionController.h"
#include "Algorithms.h"
#include "EnemiesFilter.h"
#include "Reinterpreter.h"
#include "NeuralNetwork.h"

namespace Game
{
	ComputerChampionController::ComputerChampionController(Champion* controlledChampion, Team* enemyTeam) : ChampionController(controlledChampion)
	{
		_enemyTeam = enemyTeam;
		_filter = controlledChampion->CreateFilter();
		_controlledChampion->FrameElapsed += FrameElapsed;
		FrameElapsed += std::make_pair(this, TakeTheAction);
	}

	ComputerChampionController::~ComputerChampionController()
	{
		FrameElapsed -= std::make_pair(this, TakeTheAction);
		_controlledChampion->FrameElapsed -= FrameElapsed;
		_enemyTeam = NULL;

		delete _filter;
		_filter = NULL;
	}
	
	void ComputerChampionController::TakeTheAction(Application::Object* sender, Application::EventArgs* e, Application::Object* instance)
	{
		ComputerChampionController* controller = static_cast<ComputerChampionController*>(instance);
		EnemiesFilter* filter = static_cast<EnemiesFilter*>(controller->_filter);
		if (filter->Filter(controller->_controlledChampion, controller->_enemyTeam).size() > 0)
			controller->_controlledChampion->Attack(controller->_enemyTeam);
		else
			controller->_controlledChampion->Move(Direction::Left);
	}
}