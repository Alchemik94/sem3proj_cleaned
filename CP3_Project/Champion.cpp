#ifndef _CHAMPION_INTERFACE_DEFINITION
#define _CHAMPION_INTERFACE_DEFINITION

#include "Champion.h"
#include "EnemiesFilter.h"
#include "SingleDataKeeper.h"
#include "Application.h"
#include "Reinterpreter.h"

#include <cstdio>
#include <string>

namespace Game
{
	using namespace Application;

	Champion::Champion()
	{
		_tillNextAttack = 0;
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
		FrameElapsed += std::make_pair(this, AttackCounterResetter);
	}

	Champion::Champion(ReadyPreset preset) : Champion()
	{
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

		//

		DisplayOnMap();
		_displayed = true;
	}

	Champion::~Champion()
	{
		
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
		if (_tillNextAttack > 0) return;

		EnemiesFilter* filter = static_cast<EnemiesFilter*>(CreateFilter());

		std::vector<Champion*> filteredEnemies = filter->Filter(this, enemies);

		delete filter;

		Application::VectorReinterpreter<Champion, Display::IChampionDisplayer> reinterpreter;

		if (filteredEnemies.size() > 0)
		{
			DisplayAttack(reinterpreter.Convert(filteredEnemies));
		}

		for (unsigned int i = 0; i < filteredEnemies.size(); ++i)
		{
			filteredEnemies[i]->DisplayBeingAttacked();
			filteredEnemies[i]->ChangeStatistics(CurrentHealth, Loose, GetParameter(BasicDamage));
		}

		_tillNextAttack = (SingleDataKeeper::Instance()->GetInt("ticksPerSecond")/GetParameter(ChampionParameters::AttackSpeed));
	}

	void Champion::AttackCounterResetter(Application::Object* sender, Application::EventArgs* e, Application::Object* instance)
	{
		Champion* champion = static_cast<Champion*>(instance);
		if (champion->_tillNextAttack>0)
			--champion->_tillNextAttack;
	}

	bool Champion::IsAlive()
	{
		return _currentHealth>0;
	}

	void Champion::Move(Direction direction)
	{
		if (direction == Direction::Down && GetParameter(ChampionParameters::Lane) == SingleDataKeeper::Instance()->GetInt("numberOfLanes"))
		{
			//do nothing
		}
		else if (direction == Direction::Up && GetParameter(ChampionParameters::Lane) == 1)
		{
			//do nothing
		}
		else
			ChangeStatistics(DirectionToParams(direction).first, DirectionToParams(direction).second, 1);
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