#pragma once

#include <string>
#include <vector>
#include "MenuOptionsEnum.h"
#include "EventHandler.h"
#include "IDisplayer.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

namespace Display
{
	class IMenuDisplayer: public IDisplayer
	{
		private:
			std::string _texturePath;
			ALLEGRO_FONT* _arial18;
		public:
			Application::EventHandler FrameElapsed;
			IMenuDisplayer(std::string texture)
			{
				_texturePath = texture;
			}

			virtual void Show()
			{
				_arial18 = al_load_ttf_font("arial.ttf", 30, 0);
				auto options = GetOptions();
				int i = 0;
				for (auto option : options)
				{
					if (GetMenuOptionName(option) != GetActiveOption())
						al_draw_text(_arial18, al_map_rgb(255, 255, 255), 400, 100 + (400*i) / options.size(), ALLEGRO_ALIGN_CENTER, GetMenuOptionName(option).c_str());
					else
						al_draw_text(_arial18, al_map_rgb(250, 130, 0), 400, 200, ALLEGRO_ALIGN_CENTER, GetMenuOptionName(option).c_str());
					++i;
				}
			}

			virtual void Hide()
			{
				al_destroy_font(_arial18);
			}

			virtual void Refresh()
			{
				OptionChanged();
			}

			virtual void OptionChanged()
			{
				Hide();
				Show();
			}

			//Returns active option of this menu
			virtual std::string GetActiveOption() = 0;
			//Returns all options of this menu
			virtual std::vector<Game::MenuOption>& GetOptions() = 0;
			//Decodes Game::MenuOption to string (to be easily displayed
			virtual std::string GetMenuOptionName(Game::MenuOption option) = 0;
	};
}