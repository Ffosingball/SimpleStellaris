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


std::string GetProperPlanetTypeName(PlanetType planetType)
{
	switch (planetType)
	{
	case PlanetType::BarrenDark:
	case PlanetType::BarrenGrey:
	case PlanetType::BarrenMarsLike:
		return "Barren";
	case PlanetType::VenusLike:
		return "Toxic";
	case PlanetType::Oceanic:
		return "Oceanic";
	case PlanetType::EarthLike:
		return "Earth Like";
	case PlanetType::TitanLike:
		return "Methan";
	case PlanetType::Molten:
		return "Molten";
	case PlanetType::Icy:
		return "Icy";
	case PlanetType::Voulcanic:
		return "Voulcanic";
	case PlanetType::Desert:
		return "Desert";
	case PlanetType::HotJupiter:
		return "Hot Jupiter";
	case PlanetType::HotNeptune:
		return "Hot Neptune";
	case PlanetType::JupiterLike:
	case PlanetType::SaturnLike:
		return "Large Gas Giant";
	case PlanetType::NeptuneLike:
	case PlanetType::UranusLike:
		return "Small Gas Giant";
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


std::string GetSystemTypeName(SpaceSystemType systemType)
{
	switch (systemType)
	{
	case SpaceSystemType::Single:
		return "Single";
	case SpaceSystemType::BinaryClose:
	case SpaceSystemType::BinaryAfar:
		return "Binary";
	case SpaceSystemType::TernaryAfar:
	case SpaceSystemType::TernaryTwoCloseThirdAfar:
		return "Ternary";
	}

	return "UNKNOWN";
}


std::string GetPlanetDistrictName(PlanetDistrictType districtType) 
{
	switch (districtType) 
	{
	case PlanetDistrictType::Voulcano:
		return "Voulcano";
	case PlanetDistrictType::Barren:
		return "Barren Land";
	case PlanetDistrictType::Crator:
		return "Giant Crator";
	case PlanetDistrictType::Mountains:
		return "Mountains";
	case PlanetDistrictType::Desert:
		return "Desert";
	case PlanetDistrictType::Ocean:
		return "Ocean";
	case PlanetDistrictType::Islands:
		return "Islands";
	case PlanetDistrictType::Forest:
		return "Forest";
	case PlanetDistrictType::Rainforest:
		return "Rainforest";
	case PlanetDistrictType::Steppe:
		return "Steppe";
	case PlanetDistrictType::IceSheet:
		return "Ice Sheets";
	case PlanetDistrictType::MethanOcean:
		return "Methan Ocean";
	case PlanetDistrictType::MoltenLand:
		return "Molten Land";
	}

	return "UNKNOWN";
}