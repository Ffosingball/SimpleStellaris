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
	BinaryClose,
	BinaryAfar,
	TernaryClose,
	TernaryTwoCloseThirdAfar,
	TernaryAfar
};