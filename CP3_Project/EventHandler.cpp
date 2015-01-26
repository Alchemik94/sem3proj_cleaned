#include "EventHandler.h"

namespace Application
{
	void EventHandler::operator()(Object* sender, EventArgs* e) const
	{
		auto functionIt = _functions.begin();
		auto handlerIt = _anotherHandlers.begin();
		while (functionIt != _functions.end() || handlerIt != _anotherHandlers.end())
		{
			if (functionIt == _functions.end())
				handlerIt->second(sender, e);
			else if (handlerIt == _anotherHandlers.end())
				functionIt->second(sender, e);
			else
			{
				if (functionIt->first < handlerIt->first)
					functionIt++->second(sender, e);
				else
					handlerIt++->second(sender, e);
			}
		}
	}

	const EventHandler& EventHandler::operator+=(const EventHandler& handler)
	{
		if (_functions.size() + _anotherHandlers.size() == 0)
			_anotherHandlers.push_back(std::make_pair<int, const EventHandler&>(_anotherHandlers.size() + _functions.size(), handler));
		else if (_functions.size() == 0)
			_anotherHandlers.push_back(std::make_pair<int, const EventHandler&>(_anotherHandlers.back().first + 1, handler));
		else
#define MAX(a,b) ((a)<(b) ? (b) : (a))
			_anotherHandlers.push_back(std::make_pair<int, const EventHandler&>(MAX(_anotherHandlers.back().first, _functions.back().first) + 1, handler));
#undef MAX
		return *this;
	}

	const EventHandler& EventHandler::operator+=(const Function function)
	{
		if (_functions.size() + _anotherHandlers.size() == 0)
			_functions.push_back(std::make_pair<int, const Function&>((int)(_anotherHandlers.size() + _functions.size()), function));
		else if (_anotherHandlers.size() == 0)
			_functions.push_back(std::make_pair<int, const Function&>(_functions.back().first + 1, function));
		else
#define MAX(a,b) ((a)<(b) ? (b) : (a))
			_functions.push_back(std::make_pair<int, const Function&>(MAX(_anotherHandlers.back().first, _functions.back().first) + 1, function));
#undef MAX
		return *this;
	}

	bool EventHandler::operator!=(const EventHandler& handler) const
	{
		if (this->_functions != handler._functions)
			return true;
		if (this->_anotherHandlers != handler._anotherHandlers)
			return true;
		return false;
	}

	const EventHandler& EventHandler::operator-=(const EventHandler& handler)
	{
		auto handlerIt = _anotherHandlers.begin();
		for (; handlerIt != _anotherHandlers.end() && handlerIt->second != handler; ++handlerIt);
		if (handlerIt != _anotherHandlers.end())
			_anotherHandlers.erase(handlerIt);
		return *this;
	}

	bool EventHandler::operator==(const EventHandler& handler) const
	{
		return (*this != handler) == false;
	}

	const EventHandler& EventHandler::operator-=(const Function function)
	{
		auto functionIt = _functions.begin();
		for (; functionIt != _functions.end() && functionIt->second != function; ++functionIt);
		if (functionIt != _functions.end())
			_functions.erase(functionIt);
		return *this;
	}
}