#pragma once

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


enum class SpaceSystemType
{
	Single,
	BinaryClose,
	BinaryAfar,
	BinaryCloseWithin,
	TernaryTwoCloseThirdAfar,
	TernaryAfar
};


enum class DistanceToStar 
{
	Close,
	Medium,
	Far
};