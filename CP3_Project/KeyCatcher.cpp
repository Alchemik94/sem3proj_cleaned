#include "KeyCatcher.h"
#include <cstdlib>

namespace Application
{
	KeyCatcher::KeyCatcher()
	{
		_next = NULL;
		_previous = NULL;
	}

	KeyCatcher::KeyCatcher(KeyCatcher* previous)
	{
		_next = NULL;
		_previous = previous;
	}

	KeyCatcher* KeyCatcher::ReturnControl()
	{
		if (_next == NULL)
		{
			if (_previous != NULL)
			{
				return _previous->_next = NULL;
			}
			else
			{
				return NULL;
			}
		}
		_next = _next->ReturnControl();
		return this;
	}

	void KeyCatcher::Catched(Keys key)
	{
		if (_next != NULL)
			_next->Catched(key);
		else
			CatchedKeyHandler(key);
	}
	
	void KeyCatcher::GiveControl(KeyCatcher* catcher)
	{
		if (_next == NULL)
			_next = catcher;
		else
			_next->GiveControl(catcher);
	}
}