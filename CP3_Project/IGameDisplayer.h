#pragma once

namespace Display
{
	class IGameDisplayer
	{
		public:
			IGameDisplayer()
			{
			
			}

			IGameDisplayer(int numberOfRounds)
			{

			}
			
			virtual void Show()
			{
			
			}
			
			virtual void Hide()
			{
			
			}

			virtual void Refresh()
			{

			}

			virtual int RoundsNumber() = 0;
			virtual int CurrentRound() = 0;
			
			virtual void NewRound()
			{
			
			}
	};
}