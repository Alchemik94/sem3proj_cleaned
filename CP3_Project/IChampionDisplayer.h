#pragma once

#include "GameEnums.h"
#include "SingleDataKeeper.h"
#include "EventHandler.h"
#include <vector>
#include <string>
#include "Sprite.h"
#include "IDisplayer.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


namespace Display
{
	//Interface to define graphics. ChampionType has to implement IChampionDisplayer.
	class IChampionDisplayer: public virtual IDisplayer
	{
		private:
			std::string _textureLocation;
			ALLEGRO_FONT* _arial18;
			bool _displayed;
			static void RefreshEventHandler(Application::Object* sender, Application::EventArgs* e,		Application::Object* instance)
			{
				if (dynamic_cast<IChampionDisplayer*>(instance)->_displayed)
					dynamic_cast<IChampionDisplayer*>(instance)->Refresh();
			}
		protected:
			Sprite _championImage;
		public:
			IChampionDisplayer(std::string texture)
			{
				_displayed = false;
				_textureLocation = texture;
				_championImage.maxFrame = 6;
				_championImage.curFrame = 1;
				_championImage.curRow = 1;
				_championImage.frameCount = 1;
				_championImage.frameDelay = 2;
				_championImage.frameWidth = 32;
				_championImage.frameHeight = 32;
				_championImage.animationColumns = 6;

				FrameElapsed += std::make_pair(this, RefreshEventHandler);
			}


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
				//possible effect to be added
			}
			
			//Displays change in current health
			virtual void DisplayCurrentHealthChange(Game::TypeOfChange type, int change)
			{
				//possible effect to be added
				Refresh();
			}
			
			//Displays change in maximum health
			virtual void DisplayMaximumHealthChange(Game::TypeOfChange type, int change)
			{
				//possible effect to be added
				Refresh();
			}
			
			//Shows champion on map
			virtual void DisplayOnMap()
			{
				_championImage.image = al_load_bitmap(_textureLocation.c_str());

				al_draw_bitmap_region(_championImage.image, _championImage.curFrame * _championImage.frameWidth, _championImage.curRow * _championImage.frameHeight, _championImage.frameWidth, _championImage.frameHeight, GetParameter(Game::ChampionParameters::DistanceFromCastle), (GetParameter(Game::ChampionParameters::Lane) - 1)*Application::SingleDataKeeper::Instance()->GetInt("distanceBetweenLanes") + 300, 0);

				_arial18 = al_load_ttf_font("arial.ttf", 30, 0);
				//champion healthbar
				al_draw_textf(_arial18, al_map_rgb(255, 255, 255), 20, 20, 0, "HP  %d/%d",GetParameter(Game::ChampionParameters::CurrentHealth),GetParameter(Game::ChampionParameters::MaximumHealth));

				_displayed = true;
			}

			//IDisplayer interface
			virtual void Show()
			{
				DisplayOnMap();
			}
			virtual void Hide()
			{
				al_destroy_bitmap(_championImage.image);
				al_destroy_font(_arial18);
				_displayed = false;
			}
			virtual void Refresh()
			{
				if (++_championImage.frameCount >= _championImage.frameDelay)
				{
					if (++_championImage.curFrame >= _championImage.maxFrame)
					{
						_championImage.curFrame = 0;
						if (_championImage.curRow == 2)
							_championImage.curRow = 1;
					}

					_championImage.frameCount = 0;
				}

				al_draw_bitmap_region(_championImage.image, _championImage.curFrame * _championImage.frameWidth, _championImage.curRow * _championImage.frameHeight, _championImage.frameWidth, _championImage.frameHeight, GetParameter(Game::ChampionParameters::DistanceFromCastle), (GetParameter(Game::ChampionParameters::Lane) - 1)*Application::SingleDataKeeper::Instance()->GetInt("distanceBetweenLanes") + 300, 0);

				//champion healthbar
				al_draw_textf(_arial18, al_map_rgb(255, 255, 255), GetParameter(Game::ChampionParameters::DistanceFromCastle), (GetParameter(Game::Lane) - 1)*Application::SingleDataKeeper::Instance()->GetInt("distanceBetweenLanes") + 300 - 30, 0, "HP  %d/%d", GetParameter(Game::ChampionParameters::CurrentHealth), GetParameter(Game::ChampionParameters::MaximumHealth));
			}
	
			//Returns selected champion parameter
			virtual int GetParameter(Game::ChampionParameters param) = 0;
	};
}