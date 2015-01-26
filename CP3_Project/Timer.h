#ifndef _TIMER_DECLARATION
#define _TIMER_DECLARATION

#include <time.h>
#include <thread>
#include "EventHandler.h"
#include "ITimerParam.h"

namespace Application
{
	class Timer
	{
		private:
			int _gap;
			volatile bool _running;
			std::thread* _functionRunner;
			typedef void(*Function)(ITimerParameter*);
			Function _internalFunction;
			ITimerParameter* _parameter;
			//Calls _internalFunction in a specified period of time. Ran only on _functionRunner thread.
			static void Caller(Timer* timer);
		protected:
			Timer(){}
		public:
			//Standard constructor. Defines the time timer runs in miliseconds before using the function passed as second argument.
			Timer(int msec, Function function, ITimerParameter* parameter);
			//Starts timer to perform its work.
			void Run();
			//Stops timer.
			void Stop();
			//Tells if the timer is working
			virtual bool Active()
			{
				return _running;
			}
			virtual ~Timer();
			//experimental one
			EventHandler Tick;
	};

	class EmptyTimer : private Timer
	{
		static EmptyTimer* _instance;
		public:
			EmptyTimer(){}
			static EmptyTimer Instance()
			{
				if (_instance != NULL)
					return *_instance;
				return *(_instance = new EmptyTimer());
			}
			virtual bool Active()
			{
				return true;
			}
	};
}

#endif