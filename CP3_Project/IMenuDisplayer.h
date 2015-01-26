#pragma once

#include <string>
#include <vector>
#include "MenuOptionsEnum.h"

namespace Display
{
	class IMenuDisplayer
	{
		public:
//TODO
			virtual void Show()
			{

			}

//TODO
			virtual void Hide()
			{

			}

			virtual void Refresh()
			{
				//this function must be implemented to create instance of menu - it may be like this:
				OptionChanged();
			}

//TODO
			virtual void OptionChanged()
			{

			}

			//Returns active option of this menu
			virtual std::string GetActiveOption() = 0;
			//Returns all options of this menu
			virtual std::vector<Game::MenuOption>& GetOptions() = 0;
			//Decodes Game::MenuOption to string (to be easily displayed
			virtual std::string GetMenuOptionName(Game::MenuOption option) = 0;
	};
}