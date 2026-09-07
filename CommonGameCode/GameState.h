#pragma once

//Game states, tells whether the simulation is stopped or not
enum class GameState 
{
	Paused,
	Resumed,
	Loading,
	None
};

//Difficulty levels
enum class DifficultyLevel
{
	Easy,
	Medium,
	Hard,
	None
};

//View type, what player is looking at now
enum class OverviewType
{
	Space,
	System,
	Planet,
	Always,
	None
};