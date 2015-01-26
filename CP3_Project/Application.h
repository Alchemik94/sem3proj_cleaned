#ifndef _MAIN_APPLICATION
#define _MAIN_APPLICATION

#include "SingleDataKeeper.h"
#include <atomic>

//#define LOCK_VERSION_ONE

#ifndef LOCK_VERSION_ONE
#define LOCK_APPLICATION_VARIABLES(TIMER_USED) while (Application::MainApplication::Lock.test_and_set()) if((TIMER_USED).Active() == false) return
#define UNLOCK_APPLICATION_VARIABLES Application::MainApplication::Lock.clear()
#else
#define LOCK_APPLICATION_VARIABLES while (Application::MainApplication::Lock); Application::MainApplication::Lock = true
#define UNLOCK_APPLICATION_VARIABLES Application::MainApplication::Lock = false
#endif

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
			//Multithreading lock
#ifndef LOCK_VERSION_ONE
			static std::atomic_flag Lock;
#else
			static std::atomic<bool> Lock;
#endif
	};
}
#endif