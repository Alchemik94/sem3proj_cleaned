#pragma once

#include "SingleDataKeeper.h"

namespace Application
{
	class MainApplication
	{
		public:
			//Creates application instance
			MainApplication();
			//Runs standard application actions
			int Run();
			//Tests current module
			void Test();
	};
}