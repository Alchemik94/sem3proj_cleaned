#pragma once

#include "KeyboardKeysEnum.h"

namespace Application
{
	class KeyCatcher
	{
		private:
			KeyCatcher* _next;
			KeyCatcher* _previous;
		protected:
			virtual void CatchedKeyHandler(Keys key) = 0;
		public:
			KeyCatcher();
			KeyCatcher(KeyCatcher* previous);
			void Catched(Keys key);
			KeyCatcher* ReturnControl();
			void GiveControl(KeyCatcher* catcher);
	};
}