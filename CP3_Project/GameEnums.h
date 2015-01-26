#ifndef _GAMEENUMS_DEFINITION
#define _GAMEENUMS_DEFINITION 

namespace Game
{

	enum ChampionParameters
	{
		CurrentHealth,
		MaximumHealth,
		CurrentPower,
		MaximumPower,
		Range,
		MovementSpeed,
		BasicDamage,
		AttackSpeed,
		Level,
		Experience,
		Lane,
		DistanceFromCastle,
	};

	enum TypeOfChange
	{
		Gain,
		Loose,
	};

	enum Direction
	{
		Up,
		Down,
		Left,
		Right,
		None,
	};

	enum Action
	{
		Attack,
		Move,
	};

	enum ReadyPreset
	{
		//any preset declared here have to be added as an entry in SingleDataKeeper constructor
		PlayerKnight,
		AIKnight,
	};
}

#endif