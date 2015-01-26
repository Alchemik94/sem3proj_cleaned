#include "ComputerChampionController.h"
#include "Algorithms.h"
#include "EnemiesFilter.h"
#include "Reinterpreter.h"

namespace Game
{
	ComputerChampionController::ComputerChampionController(Champion* controlledChampion, volatile bool* paused, Team* enemyTeam) : ChampionController(controlledChampion, paused)
	{
		_enemyTeam = enemyTeam;
		_filter = controlledChampion->CreateFilter();
		_timer = new Application::Timer(LowestSensiblePauseTime(controlledChampion), TakeTheAction, this);
	}

	unsigned int ComputerChampionController::LowestSensiblePauseTime(Champion* champion)
	{
		unsigned int gcd = Application::Gcd((int)((float)1 / (float)champion->GetParameter(ChampionParameters::AttackSpeed)), (int)((float)1 / (float)champion->GetParameter(ChampionParameters::MovementSpeed)));
		if (gcd != 1)
			return gcd;
		int time = Application::Min(champion->GetParameter(ChampionParameters::AttackSpeed), champion->GetParameter(ChampionParameters::MovementSpeed));
		if (time % 2 == 0)
			return time / 2;
		return time;
	}

	inline void ComputerChampionController::Start()
	{
		_timer->Run();
	}

	inline void ComputerChampionController::Stop()
	{
		_timer->Stop();
	}

	ComputerChampionController::~ComputerChampionController()
	{
		_timer->Stop();
		delete _timer;
		_enemyTeam = NULL;

		delete _filter;
		_filter = NULL;
	}
	
	void ComputerChampionController::TakeTheAction(Application::ITimerParameter* parameter)
	{
		ComputerChampionController* controller = static_cast<ComputerChampionController*>(parameter);
		if (*controller->_paused == true)
			return;
		EnemiesFilter* filter = static_cast<EnemiesFilter*>(controller->_filter);
		if (filter->Filter(controller->_controlledChampion, controller->_enemyTeam).size() > 0)
			controller->_controlledChampion->Attack(controller->_enemyTeam);
		else
			controller->_controlledChampion->Move(Direction::Left);
	}
}