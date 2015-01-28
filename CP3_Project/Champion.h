#pragma once

#include <vector>
#include "GameEnums.h"
#include "IFilter.h"
#include "IChampionDisplayer.h"
#include "EventHandler.h"

namespace Game
{
	class Champion: public virtual Display::IChampionDisplayer
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
			
			//bool letting us to make delays between attacks
			int _tillNextAttack;
			
			//function returning parameter we want to change
			int& GetChangingParameter(ChampionParameters param);
			//function obtaining modification to specified value
			int Modification(TypeOfChange type, int change);
			
			//display methods caller
			void DisplayChange(ChampionParameters param, TypeOfChange type, int change);
			
			//Changes parameters on demand
			void ChangeStatistics(ChampionParameters param, TypeOfChange type, int change);

			//used in timer to reset attack flag
			static void AttackCounterResetter(Application::Object* sender, Application::EventArgs* e, Application::Object* instance);
			
		protected:
			//Parametrized constructor allowing to create champion from ready preset.
			Champion(ReadyPreset preset);
		public:
			//abstract one, which will be defined for every derived class
			virtual Application::IFilter* CreateFilter() = 0;
			virtual ~Champion();
			virtual void Attack(std::vector<Champion*>* enemies);
			virtual void Move(Direction direction);
			bool IsAlive();
			virtual int GetParameter(ChampionParameters param);
	};
}