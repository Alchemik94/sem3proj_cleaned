#pragma once

#include "EventHandler.h"

namespace Display
{
	class IDisplayer: public virtual Application::Object
	{
		public:
			virtual void Show() = 0;
			virtual void Hide() = 0;
			virtual void Refresh() = 0;
	};
}