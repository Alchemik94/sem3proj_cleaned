#pragma once

#include "KeyboardKeysEnum.h"
#include "MenuOptionsEnum.h"
#include "GameEnums.h"
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
			static std::pair<Game::ChampionParameters, Game::TypeOfChange> DirectionToParams(Game::Direction direction)
			{
				std::pair<Game::ChampionParameters, Game::TypeOfChange> params;
				switch (direction)
				{
				case Game::Direction::Down:
					params.first = Game::Lane;
					params.second = Game::Loose;
					break;
				case Game::Direction::Left:
					params.first = Game::DistanceFromCastle;
					params.second = Game::Loose;
					break;
				case Game::Direction::Right:
					params.first = Game::DistanceFromCastle;
					params.second = Game::Gain;
					break;
				case Game::Direction::Up:
					params.first = Game::Lane;
					params.second = Game::Gain;
					break;
				case Game::Direction::None:
				default:
					throw "Unknown direction.";
					break;
				}
				return params;
			}
	};
}