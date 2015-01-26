#ifndef _IKNIGHTDISPLAYER_DECLARATION
#define _IKNIGHTDISPLAYER_DECLARATION

#include "IChampionDisplayer.h"
#include "Champion.h"
#include "GameEnums.h"
#include "Reinterpreter.h"

namespace Display
{
	class IKnightDisplayer : public virtual IChampionDisplayer
	{
		public:
			virtual void DisplayAttack(std::vector<Game::Champion*> filteredEnemies)
			{
				//TODO
			}

			virtual void DisplayAttack(std::vector<IChampionDisplayer*> filteredEnemies)
			{
				Application::VectorReinterpreter<IChampionDisplayer, Game::Champion> reinterpreter;
				DisplayAttack(reinterpreter.Convert(filteredEnemies));
			}

			virtual void DisplayDeath()
			{
				//TODO
				//when it ends, it would be great to autohide champion dying:
				Hide();
			}
			virtual void DisplayMove(Game::Direction direction, int change)
			{
				//TODO
			}
	};
}

#endif