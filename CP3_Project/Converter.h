#pragma once

#include "KeyboardKeysEnum.h"
#include "MenuOptionsEnum.h"
#include "allegro5\allegro.h"

namespace Application
{
	class Converter
	{
		public:
			static Keys AllegroKeyToAscii(int allegroKeycode)
			{
				switch (allegroKeycode)
				{
					case ALLEGRO_KEY_ESCAPE:
						return Keys::Escape;
					case ALLEGRO_KEY_DOWN:
						return Keys::Down;
					case ALLEGRO_KEY_UP:
						return Keys::Up;
					case ALLEGRO_KEY_LEFT:
						return Keys::Left;
					case ALLEGRO_KEY_RIGHT:
						return Keys::Right;
					case ALLEGRO_KEY_SPACE:
						return Keys::Space;
					default:
						return Keys::NotAKey;
				}
			}
			static std::string MenuOptionName(Game::MenuOption option)
			{
				switch (option)
				{
					case Game::MenuOption::Continue:
						return "Continue";
					case Game::MenuOption::NewGame:
						return "New Game";
					case Game::MenuOption::Options:
						return "Options";
					case Game::MenuOption::Credits:
						return "Credits";
					case Game::MenuOption::Exit:
						return "Exit";
					default:
						//throw an error
						break;
				}
			}
	};
}