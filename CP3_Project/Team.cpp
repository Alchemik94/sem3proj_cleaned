#include "Team.h"
#include "EnemiesFilter.h"
#include <vector>
#include "ChampionFactory.h"

namespace Game
{
	void Team::EraseDead(Application::Object* sender, Application::EventArgs* e, Application::Object* instance)
	{
		Team* team = dynamic_cast<Team*>(instance);
		if (team->size() > 0)
		{
			EnemiesFilter* filter = new AliveEnemiesFilter();
			std::vector<Champion*> newTeam;
			newTeam = filter->Filter(NULL, team);

			if (newTeam.size() != team->size())
			{
				for (unsigned int i = 0, j = 0; i < team->size(); ++i)
				{
					if (j<newTeam.size() && (*team)[i] == newTeam[j])
						++j;
					else
						delete (*team)[i];
				}

				*team = newTeam;
			}
		}
	}

	Team& Team::operator=(vector<Champion*> champions)
	{
		for (auto champion : *this)
			FrameElapsed -= champion->FrameElapsed;
		vector<Champion*>::operator=(champions);
		for (auto champion : *this)
			FrameElapsed += champion->FrameElapsed;
		return *this;
	}

	void Team::push_back(Champion* champion)
	{
		vector<Champion*>::push_back(champion);
		FrameElapsed += champion->FrameElapsed;
	}

	void Team::clear()
	{
		for (auto champion : *this)
			FrameElapsed -= champion->FrameElapsed;
		vector<Champion*>::clear();
	}

	void Team::pop_back()
	{
		FrameElapsed -= vector<Champion*>::back()->FrameElapsed;
		vector<Champion*>::pop_back();
	}

	Team::Team()
	{
		FrameElapsed += std::make_pair(this, EraseDead);
	}

	Team::~Team()
	{
		FrameElapsed -= std::make_pair(this, EraseDead);
		for (int i = 0; i < this->size(); ++i)
		{
			FrameElapsed -= (*this)[i]->FrameElapsed;
			delete (*this)[i];
			(*this)[i] = NULL;
		}
	}

//TODO - for example choosing random presets from the list of them
	AutogeneratingTeam::AutogeneratingTeam(int size, ReadyPreset preset) : Team()
	{
		for (int i = 0; i < size; ++i)
			this->push_back(ChampionFactory::CreateChampion(preset));
	}
}