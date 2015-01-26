#ifndef _ENEMIES_FILTER_DEFINITION
#define _ENEMIES_FILTER_DEFINITION

#include "EnemiesFilter.h"
#include "SingleDataKeeper.h"
#include <math.h>

namespace Game
{
	EnemiesFilter::EnemiesFilter()
	{
		next = NULL;
	}

	EnemiesFilter::~EnemiesFilter()
	{
		delete next;
	}

	EnemiesFilter* EnemiesFilter::Append(EnemiesFilter* filter)
	{
		if (next == NULL)
			next = filter;
		else
			next->Append(filter);
		return this;
	}

	std::vector<Champion*> EnemiesFilter::Filter(Champion* attackingChampion, std::vector<Champion*>* enemies)
	{
		if (next != NULL)
			return next->Filter(attackingChampion, enemies);
		else
			return *enemies;
	}

#define ABS(x) ((x)>=0 ? (x) : -(x))

	int DistanceEnemiesFilter::Distance(Champion* first, Champion* second)
	{
		int xDist = ABS(first->GetParameter(ChampionParameters::DistanceFromCastle) - second->GetParameter(ChampionParameters::DistanceFromCastle));

		int yDist = ABS(first->GetParameter(ChampionParameters::Lane) - second->GetParameter(ChampionParameters::Lane))*ABS(Application::SingleDataKeeper::Instance()->GetInt("distanceBetweenLanes"));

		float distance = sqrt(xDist*xDist + yDist*yDist);

		return distance > (int)distance ? (int)distance + 1 : (int)distance;
	}


#undef ABS(x)


	std::vector<Champion*> DistanceEnemiesFilter::Filter(Champion* attackingChampion, std::vector<Champion*>* enemies)
	{
		std::vector<Champion*> filtered = std::vector<Champion*>();
		for (unsigned int i = 0; i < enemies->size(); ++i)
		if (Distance(attackingChampion, (*enemies)[i]) <= attackingChampion->GetParameter(Range))
			filtered.push_back((*enemies)[i]);
		return EnemiesFilter::Filter(attackingChampion, &filtered);
	}

	std::vector<Champion*> ClosestOneEnemiesFilter::Filter(Champion* attackingChampion, std::vector<Champion*>* enemies)
	{
		Champion* closest = NULL;
		for (unsigned int i = 0; i < enemies->size(); ++i)
		if (closest == NULL || Distance(attackingChampion, (*enemies)[i]) <= Distance(attackingChampion, closest))
			closest = (*enemies)[i];
		std::vector<Champion*> filtered = std::vector<Champion*>();
		filtered.push_back(closest);
		return EnemiesFilter::Filter(attackingChampion, &filtered);
	}

	std::vector<Champion*> LaneEnemiesFilter::Filter(Champion* attackingChampion, std::vector<Champion*>* enemies)
	{
		std::vector<Champion*> filtered = std::vector<Champion*>();
		for (unsigned int i = 0; i < enemies->size(); ++i)
			if (attackingChampion->GetParameter(ChampionParameters::Lane) == (*enemies)[i]->GetParameter(ChampionParameters::Lane))
				filtered.push_back((*enemies)[i]);
		return EnemiesFilter::Filter(attackingChampion, &filtered);
	}

	std::vector<Champion*> AliveEnemiesFilter::Filter(Champion* attackingChampion, std::vector<Champion*>* enemies)
	{
		std::vector<Champion*> filtered = std::vector<Champion*>();
		for (unsigned int i = 0; i < enemies->size(); ++i)
			if ((*enemies)[i]->IsAlive())
				filtered.push_back((*enemies)[i]);
		return EnemiesFilter::Filter(attackingChampion, &filtered);
	}
}

#endif