#include "PauseMenu.h"
#include "SingleDataKeeper.h"
#include "GameHolder.h"
#include "Converter.h"

namespace Game
{
	PauseMenu::PauseMenu(Display::IGameDisplayer* gameholder, Application::KeyCatcher* masterKeyCatcher) : KeyCatcher(masterKeyCatcher), IMenuDisplayer(Application::SingleDataKeeper::Instance()->GetString("MenuBackgroundPath"))
	{
		_activeOption = GetOptions().begin();
		if (dynamic_cast<GameHolder*>(gameholder) == NULL)
		{
			throw "Menu lost gameholder.";
		}
		_gameholder = gameholder;
		Show();
	}

	PauseMenu::~PauseMenu()
	{
		Hide();
		ReturnControl();
	}

	void PauseMenu::CatchedKeyHandler(Application::Keys key)
	{
		if (key == Application::Keys::Up)
		{
			if (_activeOption == GetOptions().begin())
				_activeOption = GetOptions().end();
			--_activeOption;
			OptionChanged();
		}
		else if (key == Application::Keys::Down)
		{
			++_activeOption;
			if (_activeOption == GetOptions().end())
				_activeOption = GetOptions().begin();
			OptionChanged();
		}
		else if (key == Application::Keys::Enter)
		{
			if (*_activeOption == Continue || *_activeOption == NewGame)
			{
				ReturnControl();
				dynamic_cast<GameHolder*>(_gameholder)->EnteredGame();
			}
			else if (*_activeOption == Exit)
			{
				ReturnControl();
				static_cast<GameHolder*>(_gameholder)->Exit();
			}
		}
		else if (key == Application::Keys::Escape)
		{
			ReturnControl();
			static_cast<GameHolder*>(_gameholder)->Exit();
		}
		else
		{
			//nothing to do here
		}
	}

	std::string PauseMenu::GetActiveOption()
	{
		return Application::SingleDataKeeper::Instance()->GetString("PauseMenuOption"+Application::Converter::MenuOptionName(*_activeOption));
	}

	std::vector<MenuOption>& PauseMenu::GetOptions()
	{
		static std::vector<MenuOption> result{
										//MenuOption::Continue,
										MenuOption::NewGame,
										//MenuOption::Options,
										//MenuOption::Credits,
										MenuOption::Exit,
										};
		return result;
	}

	std::string PauseMenu::GetMenuOptionName(MenuOption option)
	{
		return Application::Converter::MenuOptionName(option);
	}
}