#pragma once

//Game states
enum class GameState 
{
	Pause,
	Game
};

//Difficulty levels
//From this level depend spawn rate of enemies,
//fire period of enemies and spawn rate of power ups
enum class DifficultyLevel
{
	Easy,
	Medium,
	Hard
};

//View type
enum class OverviewType
{
	Space,
	System,
	Planet,
	Always
};