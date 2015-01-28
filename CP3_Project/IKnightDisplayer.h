#ifndef _IKNIGHTDISPLAYER_DECLARATION
#define _IKNIGHTDISPLAYER_DECLARATION

#include "IChampionDisplayer.h"
#include "Champion.h"
#include "GameEnums.h"
#include "SingleDataKeeper.h"
#include "Reinterpreter.h"
#include "EventHandler.h"
#include <string>

namespace Display
{
	class IKnightDisplayer : public virtual IChampionDisplayer
	{
		public:
			IKnightDisplayer(std::string path) : IChampionDisplayer(path)
			{
			
			}

			virtual void DisplayAttack(std::vector<Game::Champion*> filteredEnemies)
			{
				_championImage.curRow = 2;
			}

			virtual void DisplayAttack(std::vector<IChampionDisplayer*> filteredEnemies)
			{
				Application::VectorReinterpreter<IChampionDisplayer, Game::Champion> reinterpreter;
				DisplayAttack(reinterpreter.Convert(filteredEnemies));
			}

			virtual void DisplayDeath()
			{
				//possible to add more effects
				Hide();
			}
			
			virtual void DisplayMove(Game::Direction direction, int change)
			{
				_championImage.curRow = 1;
			}
	};
}

#endif