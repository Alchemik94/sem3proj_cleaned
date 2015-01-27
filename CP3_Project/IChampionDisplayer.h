#pragma once

#include "GameEnums.h"
#include "EventHandler.h"
#include <vector>

namespace Display
{
	//Interface to define graphics. ChampionType has to implement IChampionDisplayer.
	class IChampionDisplayer
	{
		public:
			Application::EventHandler FrameElapsed;
			//Displays attack animation of a champion
			virtual void DisplayAttack(std::vector<IChampionDisplayer*> filteredEnemies) = 0;
			//Displays death of a champion and deletes him from map
			virtual void DisplayDeath() = 0;
			//Displays move of a champion
			virtual void DisplayMove(Game::Direction direction, int change) = 0;



			//Displays that champion is being attacked
			virtual void DisplayBeingAttacked()
			{
				//TODO
			}
			
			//Displays change in current health
			virtual void DisplayCurrentHealthChange(Game::TypeOfChange type, int change)
			{
				//TODO
			}
			
			//Displays change in maximum health
			virtual void DisplayMaximumHealthChange(Game::TypeOfChange type, int change)
			{
				//TODO - maybe as DisplayCurrentHealthChange if whole health bar will possibly be updated?
			}
			


			//Shows champion on map
			virtual void DisplayOnMap()
			{
				//TODO
			}



			//Returns selected champion parameter
			virtual int GetParameter(Game::ChampionParameters param) = 0;

			virtual void Show()
			{
				//I don't know - maybe like this:
				DisplayOnMap();
			}
			virtual void Hide()
			{
				//Has to be implemented
			}
			virtual void Refresh()
			{
				//Has to be implemented
			}
	};
}