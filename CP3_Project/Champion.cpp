#include "Champion.h"
#include "EnemiesFilter.h"
#include "SingleDataKeeper.h"
#include "Application.h"
#include "Reinterpreter.h"
#include "Converter.h"

#include <cstdio>
#include <string>

namespace Game
{
	using namespace Application;

	Champion::Champion(ReadyPreset preset) : IChampionDisplayer(SingleDataKeeper::Instance()->LoadPath(preset))
	{
		_tillNextAttack = 0;
		_displayed = false;
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
		
		FrameElapsed += std::make_pair(this, AttackCounterResetter);

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
				throw "Unknown modification.";
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
				throw "Unknown parameter changing.";
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

			//may be implemented in further versions
			case ChampionParameters::CurrentPower:
				break;
			case ChampionParameters::Experience:
				break;
			case ChampionParameters::Level:
				break;
			case ChampionParameters::MaximumPower:
				break;
			//not necessarily even displayed
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
		Champion* champion = dynamic_cast<Champion*>(instance);
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
			ChangeStatistics(Application::Converter::DirectionToParams(direction).first, Application::Converter::DirectionToParams(direction).second, 1);
	}
}