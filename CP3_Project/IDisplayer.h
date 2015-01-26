#pragma once

namespace Display
{
	class IDisplayer
	{
		public:
			virtual void Show() = 0;
			virtual void Hide() = 0;
			virtual void Refresh() = 0;
	};
}