#pragma once

#include <list>

namespace Application
{
	class Object
	{
		private:
			virtual void DoNothing(){}
	};

	template <class T>
	class ParametrizedObject : public Object
	{
		const T& _member;
		private:
			virtual void DoNothing(){}
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
		private:
			virtual void DoNothing(){}
	};

	template <class T>
	class ParametrizedEventArgs : public EventArgs
	{
		const T& _member;
		private:
			virtual void DoNothing(){}
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
		private:
			typedef void(*Function)(Object*, EventArgs*);
			typedef void(*ObjectFunction)(Object*, EventArgs*,Object*);
			std::list<std::pair<int,const Function> > _functions;
			std::list<std::pair<int, std::pair<Object*, ObjectFunction> > > _objectFunctions;
			std::list<std::pair<int,EventHandler*> > _anotherHandlers;
			int BiggestIndex();
		public:
			EventHandler();

			//Called when such an event occurs
			void operator()(Object* sender, EventArgs* e) const;
			
			//Adds another event handler to handle an event
			const EventHandler& operator+=(EventHandler& handler);

			//Adds another function to handle the event
			const EventHandler& operator+=(const Function function);

			//Adds another function connected with OOP object
			const EventHandler& operator+=(std::pair<Object*, const ObjectFunction> objectAndFunction);

			//Checks if the handlers are different
			bool operator!=(const EventHandler& handler) const;

			//Unregisters handler from handler
			const EventHandler& operator-=(EventHandler& handler);

			//Checks if the handlers are the same handler
			bool operator==(const EventHandler& handler) const;

			//Unregisters function from handler
			const EventHandler& operator-=(const Function function);

			//Unregusters function connected with OOP object from handler
			const EventHandler& operator-=(std::pair<Object*,const ObjectFunction> objectAndFunction);
	};
}