#include "Knight.h"
#include "EnemiesFilter.h"
#include "SingleDataKeeper.h"

namespace Game
{
	Knight::Knight(ReadyPreset preset) :Champion(preset), IKnightDisplayer(Application::SingleDataKeeper::Instance()->LoadPath(preset)), IChampionDisplayer(Application::SingleDataKeeper::Instance()->LoadPath(preset))
	{
	
	}

	Application::IFilter* Knight::CreateFilter()
	{
		return (new EnemiesFilter())->Append(new AliveEnemiesFilter())->Append(new LaneEnemiesFilter())->Append(new DistanceEnemiesFilter());
	}
}
