#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "Sprite.h"
#include <string>
#include "IDisplayer.h"
#include "EventHandler.h"
#include "SingleDataKeeper.h"

namespace Display
{
	class IGameDisplayer: IDisplayer, Application::Object
	{
		private:
			std::string _texturePath;
			Sprite _backgroundImage;
			void AllegroModulesInitialization()
			{
				al_init();
				al_install_keyboard();
				al_init_image_addon();
				al_init_primitives_addon();
				al_init_font_addon();
				al_init_ttf_addon();
			}
		public:
			Application::EventHandler FrameElapsed;
			ALLEGRO_DISPLAY* Display;
			IGameDisplayer(std::string texture)
			{
				_texturePath = texture;
				
				AllegroModulesInitialization();
				
				Display = al_create_display(Application::SingleDataKeeper::Instance()->GetInt("gameWidth"), (Application::SingleDataKeeper::Instance()->GetInt("gameHeight")));
			}
			
			virtual void Show()
			{
				_backgroundImage.image = al_load_bitmap(_texturePath.c_str());
				//rendering background
				al_draw_bitmap(_backgroundImage.image, 0, 0, 0);
			}
			
			virtual void Hide()
			{
				al_destroy_bitmap(_backgroundImage.image);
			}

			virtual void Refresh()
			{

			}

			virtual int RoundsNumber() = 0;
			virtual int CurrentRound() = 0;
			
			virtual void NewRound()
			{
			
			}

			~IGameDisplayer()
			{
				al_destroy_display(Display);
			}
	};
}