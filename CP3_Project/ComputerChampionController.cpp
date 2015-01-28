#include "ComputerChampionController.h"
#include "Algorithms.h"
#include "EnemiesFilter.h"
#include "Reinterpreter.h"
#include "NeuralNetwork.h"

namespace Game
{
	NeuralNetwork::NeuralNetwork<> ComputerChampionController::_network(Application::SingleDataKeeper::Instance()->GetString("NetPath"));

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
		auto action = controller->ConsiderOptions();
		switch (action)
		{
			case Game::Action::Attack:
				controller->_controlledChampion->Attack(controller->_enemyTeam);
				break;
			case Game::Action::Move:
				controller->_controlledChampion->Move(Direction::Left);
				break;
		}
	}

	Game::Action ComputerChampionController::ConsiderOptions()
	{
		EnemiesFilter* filter = static_cast<EnemiesFilter*>(_filter);

		long double percentageOfEnemiesInRange = ((long double)filter->Filter(_controlledChampion, _enemyTeam).size()) / ((long double)_enemyTeam->size());

		long double distanceFromCastle = ((long double)_controlledChampion->GetParameter(Game::DistanceFromCastle)) / ((long double)Application::SingleDataKeeper::Instance()->GetInt("gameWidth"));

		auto res = _network.Use(std::vector<long double>({ percentageOfEnemiesInRange,  distanceFromCastle}));
		
		if (res[0] > res[1])
			return Game::Action::Attack;
		else
			return Game::Action::Move;
	}
}