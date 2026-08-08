#pragma once
#include <string>

enum class StarType
{
	BlackHole,
	NeutronStar,
	WhiteDwarf,
	BrownDwarf,
	MredDwarf,
	KorangeDwarf,
	GsunLike,
	Ftype,
	Atype,
	Btype,
	Otype,
	RedGiant,
	RedSupergiant
};

std::string GetStarTypeName(StarType starType);

enum class SpaceObjectType
{
	Star,
	BlackHole,
	Planet,
	Asteroid,
	Nebula,
	System
};

enum class PlanetType
{
	BarrenDark,
	BarrenGrey,
	BarrenMarsLike,
	VenusLike,
	Oceanic,
	EarthLike,
	TitanLike,
	Molten,
	Icy,
	Voulcanic,
	Desert,
	HotJupiter,
	HotNeptune,
	JupiterLike,
	SaturnLike,
	NeptuneLike,
	UranusLike
};

std::string GetPlanetTypeName(PlanetType planetType);

enum class SpaceSystemType
{
	Single,
	BinaryClose,
	BinaryAfar,
	BinaryCloseWithin,
	TernaryTwoCloseThirdAfar,
	TernaryAfar
};

std::string GetSpaceSystemTypeName(SpaceSystemType systemType);

enum class DistanceToStar 
{
	Close,
	Medium,
	Far
};