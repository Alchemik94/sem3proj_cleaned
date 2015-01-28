#pragma once

#include "Champion.h"
#include "IKnightDisplayer.h"

namespace Game
{
	class Knight :public Display::IKnightDisplayer, public Champion
	{
		public:
			//Knight attacks all enemies in range on his lane.
			virtual Application::IFilter* CreateFilter();
			Knight(ReadyPreset preset);
	};
}