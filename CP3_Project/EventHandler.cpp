#include "EventHandler.h"

namespace Application
{
	EventHandler::EventHandler() : _functions(), _objectFunctions(), _anotherHandlers()
	{

	}

	void EventHandler::operator()(Object* sender, EventArgs* e) const
	{
		auto functionIt = _functions.begin();
		auto handlerIt = _anotherHandlers.begin();
		auto objectFunctionIt = _objectFunctions.begin();
		while (functionIt != _functions.end() || handlerIt != _anotherHandlers.end() || objectFunctionIt!=_objectFunctions.end())
		{
			if (functionIt == _functions.end() && handlerIt == _anotherHandlers.end())
			{
				objectFunctionIt->second.second(sender, e, objectFunctionIt->second.first);
				++objectFunctionIt;
			}
			else if (functionIt == _functions.end() && objectFunctionIt == _objectFunctions.end())
			{
				handlerIt->second->operator()(sender, e);
				++handlerIt;
			}
			else if (objectFunctionIt == _objectFunctions.end() && handlerIt == _anotherHandlers.end())
				functionIt++->second(sender, e);
			else if (functionIt == _functions.end())
			{
				if (objectFunctionIt->first < handlerIt->first)
				{
					objectFunctionIt->second.second(sender, e, objectFunctionIt->second.first);
					++objectFunctionIt;
				}
				else
				{
					handlerIt->second->operator()(sender, e);
					++handlerIt;
				}
			}
			else if (objectFunctionIt == _objectFunctions.end())
			{
				if (functionIt->first < handlerIt->first)
					functionIt++->second(sender, e);
				else
				{
					handlerIt->second->operator()(sender, e);
					++handlerIt;
				}
			}
			else if (handlerIt == _anotherHandlers.end())
			{
				if (functionIt->first < objectFunctionIt->first)
					functionIt++->second(sender, e);
				else
				{
					objectFunctionIt->second.second(sender, e, objectFunctionIt->second.first);
					++objectFunctionIt;
				}
			}
			else
			{
				if (objectFunctionIt->first < handlerIt->first)
				{
					if (objectFunctionIt->first < functionIt->first)
					{
						objectFunctionIt->second.second(sender, e, objectFunctionIt->second.first);
						++objectFunctionIt;
					}
					else
						functionIt++->second(sender, e);
				}
				else
				{
					if (handlerIt->first < functionIt->first)
					{
						handlerIt->second->operator()(sender, e);
						++handlerIt;
					}
					else
						functionIt++->second(sender, e);
				}
			}
		}
	}

	int EventHandler::BiggestIndex()
	{
#define MAX(a,b) ((a)<(b) ? (b) : (a))
		if (_functions.size() == 0 && _objectFunctions.size() == 0 && _anotherHandlers.size() == 0)
			return -1;
		if (_functions.size() == 0 && _objectFunctions.size() == 0)
			return _anotherHandlers.back().first;
		if (_functions.size() == 0 && _anotherHandlers.size() == 0)
			return _objectFunctions.back().first;
		if (_objectFunctions.size() == 0 && _anotherHandlers.size() == 0)
			return _functions.back().first;
		if (_functions.size() == 0)
			return MAX(_anotherHandlers.back().first,_objectFunctions.back().first);
		if (_anotherHandlers.size() == 0)
			return MAX(_functions.back().first, _objectFunctions.back().first);
		if (_objectFunctions.size() == 0)
			return MAX(_anotherHandlers.back().first, _functions.back().first);
		return MAX(_functions.back().first,MAX(_anotherHandlers.back().first, _objectFunctions.back().first));
#undef MAX
	}

	const EventHandler& EventHandler::operator+=(EventHandler& handler)
	{
		_anotherHandlers.push_back(std::make_pair<int, EventHandler*>(BiggestIndex()+1, &handler));
		return *this;
	}

	const EventHandler& EventHandler::operator+=(const Function function)
	{
		_functions.push_back(std::make_pair<int, const Function>(BiggestIndex()+1, (const Function&&) function));
		return *this;
	}

	const EventHandler& EventHandler::operator+=(std::pair<Object*,const ObjectFunction> objectAndFunction)
	{
		_objectFunctions.push_back(std::make_pair<int, std::pair<Object*, const ObjectFunction> >(BiggestIndex() + 1, (std::pair<Object*,const ObjectFunction>)objectAndFunction));
		return *this;
	}

	bool EventHandler::operator!=(const EventHandler& handler) const
	{
		if (this->_functions != handler._functions)
			return true;
		if (this->_anotherHandlers != handler._anotherHandlers)
			return true;
		if (this->_objectFunctions != handler._objectFunctions)
			return true;
		return false;
	}

	const EventHandler& EventHandler::operator-=(EventHandler& handler)
	{
		auto handlerIt = _anotherHandlers.begin();
		for (; handlerIt != _anotherHandlers.end() && *(handlerIt->second) != handler; ++handlerIt);
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

	const EventHandler& EventHandler::operator-=(std::pair<Object*,const ObjectFunction> objectAndFunction)
	{
		auto functionIt = _objectFunctions.begin();
		for (; functionIt != _objectFunctions.end() && (functionIt->second.first != objectAndFunction.first || functionIt->second.second != objectAndFunction.second); ++functionIt);
		if (functionIt != _objectFunctions.end())
			_objectFunctions.erase(functionIt);
		return *this;
	}
}