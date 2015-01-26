#ifndef _CHAMPION_INTERFACE_DEFINITION
#define _CHAMPION_INTERFACE_DEFINITION

#include "Champion.h"
#include "EnemiesFilter.h"
#include "SingleDataKeeper.h"
#include "Application.h"
#include "Reinterpreter.h"
#include "Timer.h"

#include <cstdio>
#include <string>

namespace Game
{
	using namespace Application;

	Champion::Champion() :_attackTimer(0, &Game::Champion::AttackCounterResetter, this), _moveHandlerTimer(0, &Game::Champion::MoveHandler, this)
	{
		_actionQueue = ActionQueue<Action, Direction>();
		_afterAttack = false;
		_displayed = false;
		_attackSpeed = 0;
		_basicDamage = 0;
		_currentHealth = 0;
		_currentPower = 0;
		_distanceFromCastle = 0;
		_experience = 0;
		_lane = 0;
		_level = 0;
		_maximumHealth = 0;
		_maximumPower = 0;
		_movementSpeed = 0;
		_range = 0;
	}

	Champion::Champion(ReadyPreset preset) : Champion()
	{
		LOCK_APPLICATION_VARIABLES(EmptyTimer::Instance());
		Application::SingleDataKeeper::Instance()->LoadPreset(
			preset,
			_attackSpeed,
			_basicDamage,
			_currentHealth,
			_currentPower,
			_distanceFromCastle,
			_experience,
			_lane,
			_level,
			_maximumHealth,
			_maximumPower,
			_movementSpeed,
			_range
			);

		_attackTimer = Application::Timer((int)(((float)1000) / ((float)_attackSpeed)), &Game::Champion::AttackCounterResetter,this);
		_moveHandlerTimer = Application::Timer((int)(((float)1000) / ((float)_movementSpeed)), &Game::Champion::MoveHandler, this);

		_attackTimer.Run();
		_moveHandlerTimer.Run();

		DisplayOnMap();
		_displayed = true;
		UNLOCK_APPLICATION_VARIABLES;
	}

	Champion::~Champion()
	{
		_attackTimer.Stop();
		_moveHandlerTimer.Stop();
		while (_wait) std::this_thread::yield();
		while (_wait) std::this_thread::yield();
	}

	int Champion::Modification(TypeOfChange type, int change)
	{
		switch (type)
		{
			case TypeOfChange::Gain:
				return change;
				break;
			case TypeOfChange::Loose:
				return -change;
				break;
			default:
//TODO
				//error throwing
				break;
		}
	}

	int& Champion::GetChangingParameter(ChampionParameters param)
	{
		switch (param)
		{
			case ChampionParameters::AttackSpeed:
				return _attackSpeed;
				break;
			case ChampionParameters::BasicDamage:
				return _basicDamage;
				break;
			case ChampionParameters::CurrentHealth:
				return _currentHealth;
				break;
			case ChampionParameters::CurrentPower:
				return _currentPower;
				break;
			case ChampionParameters::DistanceFromCastle:
				return _distanceFromCastle;
				break;
			case ChampionParameters::Experience:
				return _experience;
				break;
			case ChampionParameters::Lane:
				return _lane;
				break;
			case ChampionParameters::Level:
				return _level;
				break;
			case ChampionParameters::MaximumHealth:
				return _maximumHealth;
				break;
			case ChampionParameters::MaximumPower:
				return _maximumPower;
				break;
			case ChampionParameters::MovementSpeed:
				return _movementSpeed;
				break;
			case ChampionParameters::Range:
				return _range;
				break;
			default:
//TODO
				//error throwing
				break;
		}
	}

	int Champion::GetParameter(ChampionParameters param)
	{
		return GetChangingParameter(param);
	}

	void Champion::ChangeStatistics(ChampionParameters param, TypeOfChange type, int change)
	{
		int& parameter = GetChangingParameter(param);
		_wait = true;
		parameter += Modification(type, change);
		if (_displayed)
			DisplayChange(param,type,change);
		_wait = false;
	}

	void Champion::DisplayChange(ChampionParameters param, TypeOfChange type, int change)
	{
		Direction direction;
		switch (param)
		{
			case ChampionParameters::CurrentHealth:
				DisplayCurrentHealthChange(type,change);
				if (IsAlive() == false)
					DisplayDeath();
				break;
			case ChampionParameters::DistanceFromCastle:
				direction = type == TypeOfChange::Gain ? Direction::Right : Direction::Left;
				DisplayMove(direction,change);
				break;
			case ChampionParameters::Lane:
				direction = type == TypeOfChange::Gain ? Direction::Up : Direction::Down;
				DisplayMove(direction, change);
				break;
			case ChampionParameters::MaximumHealth:
				DisplayMaximumHealthChange(type,change);
				break;
//TODO
			//may be implemented in further versions
			case ChampionParameters::CurrentPower:
				break;
			case ChampionParameters::Experience:
				break;
			case ChampionParameters::Level:
				break;
			case ChampionParameters::MaximumPower:
				break;
			//not necessarily displayed
			case ChampionParameters::AttackSpeed:
			case ChampionParameters::BasicDamage:
			case ChampionParameters::MovementSpeed:
			case ChampionParameters::Range:
			default:
				break;
		}
	}

	void Champion::Attack(std::vector<Champion*>* enemies)
	{
		while (_afterAttack) std::this_thread::yield();

		EnemiesFilter* filter = static_cast<EnemiesFilter*>(CreateFilter());

		std::vector<Champion*> filteredEnemies = filter->Filter(this, enemies);

		delete filter;

		Application::VectorReinterpreter<Champion, Display::IChampionDisplayer> reinterpreter;

		if (filteredEnemies.size() > 0)
		{
			LOCK_APPLICATION_VARIABLES(EmptyTimer::Instance());
			DisplayAttack(reinterpreter.Convert(filteredEnemies));
			UNLOCK_APPLICATION_VARIABLES;
		}

		for (unsigned int i = 0; i < filteredEnemies.size(); ++i)
		{
			LOCK_APPLICATION_VARIABLES(EmptyTimer::Instance());
			filteredEnemies[i]->DisplayBeingAttacked();
			filteredEnemies[i]->ChangeStatistics(CurrentHealth, Loose, GetParameter(BasicDamage));
			UNLOCK_APPLICATION_VARIABLES;
		}

		LOCK_APPLICATION_VARIABLES(EmptyTimer::Instance());
		_afterAttack = true;
		UNLOCK_APPLICATION_VARIABLES;
	}

	void Champion::AttackCounterResetter(ITimerParameter* champ)
	{
		Champion* champion = static_cast<Champion*>(champ);
		LOCK_APPLICATION_VARIABLES(champion->_attackTimer);
		champion->_afterAttack = false;
		UNLOCK_APPLICATION_VARIABLES;
	}

	bool Champion::IsAlive()
	{
		return _currentHealth>0;
	}

	void Champion::Move(Direction direction)
	{
		LOCK_APPLICATION_VARIABLES(EmptyTimer::Instance());
		_actionQueue.Push(Action::Move, direction);
		UNLOCK_APPLICATION_VARIABLES;
	}

	void Champion::MoveHandler(ITimerParameter* champ)
	{
		Champion* champion = static_cast<Champion*>(champ);
		if (champion->_actionQueue.IsEmpty())
			return;
		LOCK_APPLICATION_VARIABLES(champion->_moveHandlerTimer);
		std::pair<Action, Direction> top = champion->_actionQueue.Pop();
		UNLOCK_APPLICATION_VARIABLES;
		if (top.first != Action::Move)
			return;
		ChampionParameters param = DirectionToParams(top.second).first;
		TypeOfChange type = DirectionToParams(top.second).second;

		LOCK_APPLICATION_VARIABLES(champion->_moveHandlerTimer);
		champion->ChangeStatistics(param, type, 1);
		UNLOCK_APPLICATION_VARIABLES;
	}

	std::pair<ChampionParameters, TypeOfChange> Champion::DirectionToParams(Direction direction)
	{
		std::pair<ChampionParameters, TypeOfChange> params;
		switch (direction)
		{
			case Direction::Down:
				params.first = Lane;
				params.second = Loose;
				break;
			case Direction::Left:
				params.first = DistanceFromCastle;
				params.second = Loose;
				break;
			case Direction::Right:
				params.first = DistanceFromCastle;
				params.second = Gain;
				break;
			case Direction::Up:
				params.first = Lane;
				params.second = Gain;
				break;
			case Direction::None:
			default:
//TODO
				//throw an error!
				break;
		}
		return params;
	}
}

#endif