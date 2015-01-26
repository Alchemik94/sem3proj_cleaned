#ifndef _CHAMPION_INTERFACE
#define _CHAMPION_INTERFACE

#include <vector>
#include "GameEnums.h"
#include "IFilter.h"
#include "ActionQueue.h"
#include "Timer.h"
#include "ITimerParam.h"
#include "IChampionDisplayer.h"

namespace Game
{
	class Champion: Application::ITimerParameter, protected virtual Display::IChampionDisplayer
	{
		private:
			int _currentHealth,
				_maximumHealth,
				_currentPower,
				_maximumPower,
				_range,
				_movementSpeed,
				_basicDamage,
				_attackSpeed,
				_level,
				_experience,
				_lane,
				_distanceFromCastle;
			bool _displayed, _wait;
			
			//queue used to move champion
			ActionQueue<Action, Direction> _actionQueue;
			//bool letting us to make delays between attacks
			volatile bool _afterAttack;
			//timer resetting _afterAttack flag
			Application::Timer _attackTimer;
			//timer moving champion with specified speed
			Application::Timer _moveHandlerTimer;

			//function returning parameter we want to change
			int& GetChangingParameter(ChampionParameters param);
			//function obtaining modification to specified value
			int Modification(TypeOfChange type, int change);
			
			//display methods caller
			void DisplayChange(ChampionParameters param, TypeOfChange type, int change);
			
			//standard constructor which is internally used for primary initialization
			Champion();

			//Changes parameters on demand
			void ChangeStatistics(ChampionParameters param, TypeOfChange type, int change);

			//used in timer to move champion graphics
			static void MoveHandler(ITimerParameter* champion);
			//used in timer to reset attack flag
			static void AttackCounterResetter(ITimerParameter* champion);
			
//TODO
			//should be moved to kind of enum converter
			static std::pair<ChampionParameters, TypeOfChange> DirectionToParams(Direction direction);
		protected:
			//Parametrized constructor allowing to create champion from ready preset.
			Champion(ReadyPreset preset);
		public:
			//abstract one, which will be defined for every derived class
			virtual Application::IFilter* CreateFilter() = 0;
			virtual ~Champion();
//TODO
			//need to be reworked
			virtual void Attack(std::vector<Champion*>* enemies);
			virtual void Move(Direction direction);
			bool IsAlive();
			virtual int GetParameter(ChampionParameters param);
	};
}

#endif