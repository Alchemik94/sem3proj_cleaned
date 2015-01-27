#ifndef _SINGLE_DATA_KEEPER
#define _SINGLE_DATA_KEEPER

#include <unordered_map>
#include <string>
#include "GameEnums.h"
#include "KeyboardKeysEnum.h"
#include "PauseMenu.h"

namespace Application
{
	class SingleDataKeeper
	{
		private:
			//singleton structure
			SingleDataKeeper()
			{
				//Some constants used in program
				KeepInt("distanceBetweenLanes", 20);
				KeepInt("ticksPerSecond", 60);
				KeepInt("numberOfLanes", 1);

				//Key bindings
				KeepInt("UpKey",Keys::Up);
				KeepInt("DownKey",Keys::Down);
				KeepInt("LeftKey",Keys::Left);
				KeepInt("RightKey",Keys::Right);
				KeepInt("AttackKey",Keys::Space);
				KeepInt("PauseKey",Keys::Escape);
				
				//Menu options names
				KeepString("PauseMenuOption" + Game::PauseMenu::MenuOptionName(Game::MenuOption::Continue),"Continue");
				KeepString("PauseMenuOption" + Game::PauseMenu::MenuOptionName(Game::MenuOption::Credits), "Credits");
				KeepString("PauseMenuOption" + Game::PauseMenu::MenuOptionName(Game::MenuOption::Exit), "Exit");
				KeepString("PauseMenuOption" + Game::PauseMenu::MenuOptionName(Game::MenuOption::NewGame), "New game");
				KeepString("PauseMenuOption" + Game::PauseMenu::MenuOptionName(Game::MenuOption::Options), "Options");

				KeepString("BackgroundImagePath", "background.png");
				KeepString("MenuBackgroundPath", "menu.png");

				//To change
				SavePreset(Game::ReadyPreset::AIKnight, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				SavePath(Game::ReadyPreset::AIKnight, "mob.png");
				SavePreset(Game::ReadyPreset::PlayerKnight, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				SavePath(Game::ReadyPreset::PlayerKnight, "champion.png");
			}
			~SingleDataKeeper();
			static SingleDataKeeper* _instance;
			static bool _initialized;
			//dictionary
			std::unordered_map<std::string,std::string> _stringDictionary;
			std::unordered_map<std::string, int> _intDictionary;
			void InitializeComponent();
		public:
			//Gives access to single instance in whole program
			static SingleDataKeeper* Instance();
			//Keeps string value under specified name
			void KeepString(std::string name, std::string value);
			//Keeps int value under specified name
			void KeepInt(std::string name, int value);
			//Gets string value from specified name
			const std::string GetString(std::string name);
			//Gets int value from specified name
			const int GetInt(std::string name);
			//Checks if specified name of string exists
			const bool ContainsString(std::string name) const;
			//Checks if specified name of int exists
			const bool ContainsInt(std::string name) const;

			//Facade for saving presets
			void SavePreset(
				Game::ReadyPreset preset,
				int attackSpeed,
				int basicDamage,
				int currentHealth,
				int currentPower,
				int distanceFromCastle,
				int experience,
				int lane,
				int level,
				int maximumHealth,
				int maximumPower,
				int movementSpeed,
				int range
				);
			//Facade for loading presents
			const void LoadPreset(
				Game::ReadyPreset preset,
				int& attackSpeed,
				int& basicDamage,
				int& currentHealth,
				int& currentPower,
				int& distanceFromCastle,
				int& experience,
				int& lane,
				int& level,
				int& maximumHealth,
				int& maximumPower,
				int& movementSpeed,
				int& range
				);
			void SavePath(Game::ReadyPreset preset,std::string path);
			std::string LoadPath(Game::ReadyPreset preset);
	};
}

#endif