#include <SFML/Graphics.hpp>
#include <memory>
#include "SceneNode.h"
#include "ECSGame.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Components.h"
#include <iostream>
#include "ResourceManager.h"
#include "CommonGameCode.h"
#include "SpaceObjectTypes.h"
#include "SpaceMapConfiguration.h"
#include "GetTextureNameFunctions.h"

std::string GetSystemTextureName(StarType starType)
{
	switch (starType)
	{
	case StarType::BlackHole:
		return "BlackHole";
		break;
	case StarType::NeutronStar:
		return "NeutronStarSystem";
		break;
	case StarType::WhiteDwarf:
		return "WhiteDwarfSystem";
		break;
	case StarType::BrownDwarf:
		return "BrownDwarfSystem";
		break;
	case StarType::MredDwarf:
		return "MclassSystem";
		break;
	case StarType::KorangeDwarf:
		return "KclassSystem";
		break;
	case StarType::GsunLike:
		return "GclassSystem";
		break;
	case StarType::Ftype:
		return "FclassSystem";
		break;
	case StarType::Atype:
		return "AclassSystem";
		break;
	case StarType::Btype:
		return "BclassSystem";
		break;
	case StarType::Otype:
		return "OclassSystem";
		break;
	case StarType::RedGiant:
		return "RedGiantSystem";
		break;
	case StarType::RedSupergiant:
		return "RedSupergiantSystem";
		break;
	}

	return "Placeholder";
}


std::string GetPlanetIconTextureName(PlanetType planetType, float planetSize, SpaceMapConfigurations& mapConfig, std::weak_ptr<HabitablePlanetComponent> wpHabitablePlanet)
{
	switch (planetType)
	{
	case PlanetType::BarrenDark:
		if (planetSize < mapConfig.smallRockyPlanetSizes.y)
			return "SmallDarkBarrenPlanetIcon";
		else if (planetSize < mapConfig.mediumRockyPlanetSizes.y)
			return "MediumDarkBarrenPlanetIcon";
		else
			return "LargeDarkBarrenPlanetIcon";
	case PlanetType::BarrenGrey:
		if (planetSize < mapConfig.smallRockyPlanetSizes.y)
			return "SmallGreyBarrenPlanetIcon";
		else if (planetSize < mapConfig.mediumRockyPlanetSizes.y)
			return "MediumGreyBarrenPlanetIcon";
		else
			return "LargeGreyBarrenPlanetIcon";
	case PlanetType::BarrenMarsLike:
		if (planetSize < mapConfig.smallRockyPlanetSizes.y)
			return "SmallRedBarrenPlanetIcon";
		else if (planetSize < mapConfig.mediumRockyPlanetSizes.y)
			return "MediumRedBarrenPlanetIcon";
		else
			return "LargeRedBarrenPlanetIcon";
	case PlanetType::VenusLike:
		if (planetSize < mapConfig.smallRockyPlanetSizes.y)
			return "SmallVenusLikePlanetIcon";
		else if (planetSize < mapConfig.mediumRockyPlanetSizes.y)
			return "MediumVenusLikePlanetIcon";
		else
			return "LargeVenusLikePlanetIcon";
	case PlanetType::Oceanic:
		if (planetSize < mapConfig.smallIcyPlanetSizes.y)
			return "SmallOceanicPlanetIcon";
		else if (planetSize < mapConfig.mediumIcyPlanetSizes.y)
			return "MediumOceanicPlanetIcon";
		else
			return "LargeOceanicPlanetIcon";
	case PlanetType::EarthLike:
		if (planetSize < mapConfig.smallRockyPlanetSizes.y)
		{
			if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Close)
				return "SmallCloseEarthLikePlanetIcon";
			else if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Medium)
				return "SmallEarthLikePlanetIcon";
			else
				return "SmallAfarEarthLikePlanetIcon";
		}
		else if (planetSize < mapConfig.mediumRockyPlanetSizes.y)
		{
			if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Close)
				return "MediumCloseEarthLikePlanetIcon";
			else if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Medium)
				return "MediumEarthLikePlanetIcon";
			else
				return "MediumAfarEarthLikePlanetIcon";
		}
		else
		{
			if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Close)
				return "LargeCloseEarthLikePlanetIcon";
			else if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Medium)
				return "LargeEarthLikePlanetIcon";
			else
				return "LargeAfarEarthLikePlanetIcon";
		}
	case PlanetType::TitanLike:
		return "TitanLikePlanetIcon";
	case PlanetType::Molten:
		if (planetSize < mapConfig.smallRockyPlanetSizes.y)
			return "SmallMoltenPlanetIcon";
		else if (planetSize < mapConfig.mediumRockyPlanetSizes.y)
			return "MediumMoltenPlanetIcon";
		else
			return "LargeMoltenPlanetIcon";
	case PlanetType::Icy:
		if (planetSize < mapConfig.smallIcyPlanetSizes.y)
			return "SmallIcyPlanetIcon";
		else if (planetSize < mapConfig.mediumIcyPlanetSizes.y)
			return "MediumIcyPlanetIcon";
		else
			return "LargeIcyPlanetIcon";
	case PlanetType::Voulcanic:
		return "VoulcanicPlanetIcon";
	case PlanetType::Desert:
		if (planetSize < mapConfig.smallRockyPlanetSizes.y)
			return "SmallDesertPlanetIcon";
		else if (planetSize < mapConfig.mediumRockyPlanetSizes.y)
			return "MediumDesertPlanetIcon";
		else
			return "LargeDesertPlanetIcon";
	case PlanetType::HotJupiter:
		return "HotJupiterPlanetIcon";
	case PlanetType::HotNeptune:
		return "HotNeptunePlanetIcon";
	case PlanetType::JupiterLike:
		return "JupiterLikePlanetIcon";
	case PlanetType::SaturnLike:
		return "SaturnLikePlanetIcon";
	case PlanetType::NeptuneLike:
		return "NeptuneLikePlanetIcon";
	case PlanetType::UranusLike:
		return "UranusLikePlanetIcon";
	}

	return "Placeholder";
}


std::string GetPlanetTextureName(PlanetType planetType, std::weak_ptr<HabitablePlanetComponent> wpHabitablePlanet)
{
	switch (planetType)
	{
	case PlanetType::BarrenDark:
		return "DarkBarren";
	case PlanetType::BarrenGrey:
		return "GreyBarren";
	case PlanetType::BarrenMarsLike:
		return "RedBarren";
	case PlanetType::VenusLike:
		return "VenusLike";
	case PlanetType::Oceanic:
		return "Oceanic";
	case PlanetType::EarthLike:
		if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Close)
			return "CloseEarthLike";
		else if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Medium)
			return "EarthLike";
		else
			return "AfarEarthLike";
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

	return "Placeholder";
}


std::string GetRingIconTextureName(PlanetType planetType, float planetSize, SpaceMapConfigurations& mapConfig)
{
	switch (planetType)
	{
	case PlanetType::BarrenDark:
	case PlanetType::BarrenGrey:
	case PlanetType::BarrenMarsLike:
	case PlanetType::VenusLike:
	case PlanetType::EarthLike:
	case PlanetType::Molten:
	case PlanetType::Desert:
		if (planetSize < mapConfig.smallRockyPlanetSizes.y)
			return "RingIconSmallRocky";
		else if (planetSize < mapConfig.mediumRockyPlanetSizes.y)
			return "RingIconMediumRocky";
		else
			return "RingIconLargeRocky";
	case PlanetType::Icy:
	case PlanetType::Oceanic:
		if (planetSize < mapConfig.smallIcyPlanetSizes.y)
			return "RingIconSmallRocky";
		else if (planetSize < mapConfig.mediumIcyPlanetSizes.y)
			return "RingIconMediumRocky";
		else
			return "RingIconLargeRocky";
	case PlanetType::Voulcanic:
	case PlanetType::TitanLike:
		return "RingIconSmallRocky";
	case PlanetType::HotJupiter:
	case PlanetType::HotNeptune:
	case PlanetType::JupiterLike:
	case PlanetType::SaturnLike:
	case PlanetType::NeptuneLike:
	case PlanetType::UranusLike:
		if (planetSize < mapConfig.largeGasSizes.x)
			return "RingIconSmallGas";
		else
			return "RingIconLargeGas";
	}

	return "Placeholder";
}