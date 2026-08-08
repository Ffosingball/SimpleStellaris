#include "SpaceObjectTypes.h"
#include <string>

std::string GetStarTypeName(StarType starType) 
{
	switch (starType) 
	{
	case StarType::BlackHole:
		return "Black Hole";
	case StarType::NeutronStar:
		return "Neutron Star";
	case StarType::WhiteDwarf:
		return "White Dwarf";
	case StarType::BrownDwarf:
		return "Brown Dwarf";
	case StarType::MredDwarf:
		return "M class";
	case StarType::KorangeDwarf:
		return "K class";
	case StarType::GsunLike:
		return "G class";
	case StarType::Ftype:
		return "F class";
	case StarType::Atype:
		return "A class";
	case StarType::Btype:
		return "B class";
	case StarType::Otype:
		return "O class";
	case StarType::RedGiant:
		return "Red giant";
	case StarType::RedSupergiant:
		return "Red Supergiant";
	}

	return "UNKNOWN";
}


std::string GetPlanetTypeName(PlanetType planetType) 
{
	switch (planetType) 
	{
	case PlanetType::BarrenDark:
		return "BarrenDark";
	case PlanetType::BarrenGrey:
		return "BarrenGrey";
	case PlanetType::BarrenMarsLike:
		return "BarrenRed";
	case PlanetType::VenusLike:
		return "VenusLike";
	case PlanetType::Oceanic:
		return "Oceanic";
	case PlanetType::EarthLike:
		return "EarthLike";
	case PlanetType::TitanLike:
		return "TitanLike";
	case PlanetType::Molten:
		return "Molten";
	case PlanetType::Icy:
		return "Icy";
	case PlanetType::Voulcanic:
		return "Voulcanic";
	case PlanetType::Desert:
		return "Desert";
	case PlanetType::HotJupiter:
		return "HotJupiter";
	case PlanetType::HotNeptune:
		return "HotNeptune";
	case PlanetType::JupiterLike:
		return "JupiterLike";
	case PlanetType::SaturnLike:
		return "SaturnLike";
	case PlanetType::NeptuneLike:
		return "NeptuneLike";
	case PlanetType::UranusLike:
		return "UranusLike";
	}

	return "UNKNOWN";
}


std::string GetSpaceSystemTypeName(SpaceSystemType spaceSystemType)
{
	switch (spaceSystemType) 
	{
	case SpaceSystemType::Single:
		return "Single";
	case SpaceSystemType::BinaryClose:
		return "BinaryClose";
	case SpaceSystemType::BinaryAfar:
		return "BinaryAfar";
	case SpaceSystemType::BinaryCloseWithin:
		return "BinaryCloseWithin";
	case SpaceSystemType::TernaryAfar:
		return "TernaryAfar";
	case SpaceSystemType::TernaryTwoCloseThirdAfar:
		return "TernaryTwoBinaryOneAfar";
	}

	return "UNKNOWN";
}