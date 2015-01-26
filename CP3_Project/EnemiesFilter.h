#pragma once

#include "Champion.h"
#include "IFilter.h"

namespace Game
{
	class EnemiesFilter: public Application::IFilter
	{
		private:
			EnemiesFilter* next;
		public:
			EnemiesFilter();
			//Adds next filter to the chain
			EnemiesFilter* Append(EnemiesFilter* filter);
			//Filters enemies with respect to current filter type
			virtual std::vector<Champion*> Filter(Champion* attackingChampion, std::vector<Champion*>* enemies);
			virtual ~EnemiesFilter();
	};

	class DistanceEnemiesFilter : public EnemiesFilter
	{
		protected:
			virtual int Distance(Champion* first, Champion* second);
		public:
			virtual std::vector<Champion*> Filter(Champion* attackingChampion, std::vector<Champion*>* enemies);
	};

	class ClosestOneEnemiesFilter : public DistanceEnemiesFilter
	{
		public:
			virtual std::vector<Champion*> Filter(Champion* attackingChampion, std::vector<Champion*>* enemies);
	};

	class LaneEnemiesFilter : public EnemiesFilter
	{
		public:
			virtual std::vector<Champion*> Filter(Champion* attackingChampion, std::vector<Champion*>* enemies);
	};

	class AliveEnemiesFilter: public EnemiesFilter
	{
		public:
			virtual std::vector<Champion*> Filter(Champion* attackingChampion, std::vector<Champion*>* enemies);
	};
}