#ifndef _TIMER_DEFINITION
#define _TIMER_DEFINITION

#include "Timer.h"
#include <Windows.h>

namespace Application
{
	Timer::Timer(int msec, Function function, ITimerParameter* parameter)
	{
		_gap = msec;
		_internalFunction = function;
		_parameter = parameter;
		_running = false;
	}

	void Timer::Caller(Timer* timer)
	{
		while (timer->_running)
		{
			//Sleep(timer->_gap);
			std::this_thread::sleep_for(std::chrono::milliseconds(timer->_gap));
			timer->_internalFunction(timer->_parameter);
			Object* sender = new ParametrizedObject<Timer>(*timer);
			EventArgs* eventArgs = new ParametrizedEventArgs<int>(0);
			timer->Tick(sender, eventArgs);
			delete sender;
			delete eventArgs;
		}
	}

	void Timer::Run()
	{
		//avoids to loose the handle to thread and memory leakage
		if (_running) return;

		_running = true;
		_functionRunner = new std::thread(Caller,this);
	}

	void Timer::Stop()
	{
		//avoids access violations
		if (_running == false) return;
		
		_running = false;
		if (_functionRunner->joinable())
		_functionRunner->join();
		_functionRunner = NULL;
//		delete _functionRunner;
	}

	Timer::~Timer()
	{
		Stop();
	}

	EmptyTimer* EmptyTimer::_instance;
}

#endif