#ifndef _APPLICATION_INITIALIZER
#define _APPLICATION_INITIALIZER

#include <atomic>
#include "Application.h"

#ifdef _DEBUG
	#include <iostream>
	#include <Windows.h>
	#include "GameHolder.h"
#endif

namespace Application
{
#ifndef LOCK_VERSION_ONE
	std::atomic_flag MainApplication::Lock = ATOMIC_FLAG_INIT;
#else
	std::atomic<bool> MainApplication::Lock(false);
#endif

	MainApplication::MainApplication()
	{
		
	}

	int MainApplication::Run()
	{
		return 0;
	}

	void MainApplication::Test()
	{
		using namespace Game;
		GameHolder* game = new GameHolder();
		game->Catched(Keys::Escape);
		game->Catched(Keys::Up);
		game->Catched(Keys::Enter);
		while (game->Running())
		{
			
		}
		delete game;
	}
}

#endif