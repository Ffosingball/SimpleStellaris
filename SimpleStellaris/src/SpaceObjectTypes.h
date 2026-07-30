#pragma once

enum class StarType
{
	WhiteDwarf,
	NeutronStar,
	MainSequenceStar,
	Giant,
	SuperGiant
};

enum class SpaceObjectType
{
	Star,
	BlackHole,
	Planet,
	Asteroid
};

enum class PlanetType
{
	Barren,
	VenusLike,
	OceanPlanet,
	EarthLike,
	MethanPlanet,
	MoltenPlanet,
	Giant,
	HotGiant
};


enum class SpaceSystemType
{
	Single,
	Binary,
	Ternary,
	Planetary
};