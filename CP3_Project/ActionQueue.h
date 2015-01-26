#ifndef _ACTION_QUEUE_DECLARATION
#define _ACTION_QUEUE_DECLARATION

#include <queue>

namespace Game
{
	template <class ActionType, class DirectionalType>
	class ActionQueue
	{
		private:
			std::queue<std::pair<ActionType,DirectionalType>> _queue;
		public:
			//ActionQueue<ActionType, DirectionalType>(){}
			void Push(ActionType action, DirectionalType direction)
			{
				_queue.push(std::make_pair(action, direction));
			}

			std::pair<ActionType, DirectionalType> Pop()
			{
				auto top = _queue.front();
				_queue.pop();
				return top;
			}

			bool IsEmpty()
			{
				return _queue.empty();
			}
	};
}

#endif