//Antony Saputra D00197615

#pragma once

//Entity/SceneNode category, used to dispatch commands
enum class CategoryID
{
	None = 0,
	Scene = 1 << 0,
	PlayerStickman1 = 1 << 1,
	PlayerStickman2 = 1 << 2,
	EnemyAircraft = 1 << 3,
	mainIsland = 1 << 4,
	Mini_Island1 = 1 << 5,
	Mini_Island2 = 1 << 6,
	SoundEffect = 1 << 7,

	Island = mainIsland | Mini_Island1 | Mini_Island2,
};