#ifndef _TEAM_DECLARATION
#define _TEAM_DECLARATION

#include "Champion.h"
#include <vector>
#include "GameEnums.h"
#include "EventHandler.h"

namespace Game
{
	class Team: public std::vector<Champion*>, public Application::Object
	{
		private:
			static void EraseDead(Application::Object* sender, Application::EventArgs* e, Application::Object* instance);
		public:
			Application::EventHandler FrameElapsed;
			void push_back(Champion* champion);
			void clear();
			void pop_back();
			Team& operator=(vector<Champion*> champions);
			Team();
			~Team();
	};

	class AutogeneratingTeam : public Team
	{
		public:
			AutogeneratingTeam(int size, ReadyPreset preset);
	};
}

#endif