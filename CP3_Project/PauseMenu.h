#pragma once

#include "IMenuDisplayer.h"
#include "KeyCatcher.h"
#include "MenuOptionsEnum.h"
#include <vector>
#include "IGameDisplayer.h"

namespace Game
{
	class PauseMenu: public Display::IMenuDisplayer, public Application::KeyCatcher
	{
		protected:
			virtual void CatchedKeyHandler(Application::Keys key);
			std::vector<MenuOption>::iterator _activeOption;
			Display::IGameDisplayer* _gameholder;
		public:
			PauseMenu(Display::IGameDisplayer* gameholder, Application::KeyCatcher* masterKeyCatcher);
			virtual std::string GetActiveOption();
			virtual std::vector<MenuOption>& GetOptions();
			virtual std::string GetMenuOptionName(MenuOption option);
			~PauseMenu();
	};
}