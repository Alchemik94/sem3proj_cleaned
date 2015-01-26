#pragma once

#include <list>

namespace Application
{
	class Object
	{

	};

	template <class T>
	class ParametrizedObject : public Object
	{
		const T& _member;
		public:
			ParametrizedObject(const T& object) : _member(object)
		{
			
		}
		const T& Value()
		{
			return _member;
		}
	};

	class EventArgs: public Object
	{

	};

	template <class T>
	class ParametrizedEventArgs : public EventArgs
	{
		const T& _member;
		public:
			ParametrizedEventArgs(const T& eventArgs) : _member(eventArgs)
			{
				
			}
			const T& Value()
			{
				return _member;
			}
	};

	class EventHandler
	{
		typedef void(*Function)(Object*, EventArgs*);
		std::list<std::pair<int,const Function> > _functions;
		std::list<std::pair<int,const EventHandler&> > _anotherHandlers;
		public:
			//Called when such an event occurs
			void operator()(Object* sender, EventArgs* e) const;
			
			//Adds another event handler to handle an event
			const EventHandler& operator+=(const EventHandler& handler);

			//Adds another function to handle the event
			const EventHandler& operator+=(const Function function);

			//Checks if the handlers are different
			bool operator!=(const EventHandler& handler) const;

			//Unregisters handler from handler
			const EventHandler& operator-=(const EventHandler& handler);

			//Checks if the handlers are the same handler
			bool operator==(const EventHandler& handler) const;

			//Unregisters function from handler
			const EventHandler& operator-=(const Function function);
	};
}