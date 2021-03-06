//Antony Saputra D00197615
//Conor Byrne D00185566

#include "DataTable.hpp"
#include "Stickman.hpp"
#include "PlayerID.hpp"
//#include "Pickup.hpp"
#include "StickmanID.hpp"
#include "IslandID.hpp"
//#include "PickupID.hpp"


std::vector<PlayerData> initializePlayerData()
{
	std::vector<PlayerData> data(static_cast<int>(PlayerID::TypeCount));
	data[static_cast<int>(PlayerID::Player1)].mKeyBinding[sf::Keyboard::A] = ActionID::MoveLeft;
	data[static_cast<int>(PlayerID::Player1)].mKeyBinding[sf::Keyboard::D] = ActionID::MoveRight;
	data[static_cast<int>(PlayerID::Player1)].mKeyBinding[sf::Keyboard::W] = ActionID::Jump;
	data[static_cast<int>(PlayerID::Player1)].mKeyBinding[sf::Keyboard::Space] = ActionID::Punch;
	data[static_cast<int>(PlayerID::Player1)].categoryID = CategoryID::PlayerStickman1;

	data[static_cast<int>(PlayerID::Player2)].mKeyBinding[sf::Keyboard::Left] = ActionID::MoveLeft;
	data[static_cast<int>(PlayerID::Player2)].mKeyBinding[sf::Keyboard::Right] = ActionID::MoveRight;
	data[static_cast<int>(PlayerID::Player2)].mKeyBinding[sf::Keyboard::Up] = ActionID::Jump;
	data[static_cast<int>(PlayerID::Player2)].mKeyBinding[sf::Keyboard::Enter] = ActionID::Punch;
	data[static_cast<int>(PlayerID::Player2)].categoryID = CategoryID::PlayerStickman2;


	return data;
}


std::vector<StickmanData> initializeStickmanData()
{
	std::vector<StickmanData> data(static_cast<int>(StickmanID::TypeCount));

	data[static_cast<int>(StickmanID::BlueStickman)].damageMultiplier = 1.0f;
	data[static_cast<int>(StickmanID::BlueStickman)].speed = 400.f;
	data[static_cast<int>(StickmanID::BlueStickman)].punchInterval = sf::seconds(0.5f);
	data[static_cast<int>(StickmanID::BlueStickman)].maxAirTime = sf::seconds(0.5f);
	data[static_cast<int>(StickmanID::BlueStickman)].texture = TextureID::BlueStick;
	data[static_cast<int>(StickmanID::BlueStickman)].runRight = TextureID::RunningRight;
	data[static_cast<int>(StickmanID::BlueStickman)].runLeft = TextureID::RunningLeft;
	data[static_cast<int>(StickmanID::BlueStickman)].jump = TextureID::Jump;
	data[static_cast<int>(StickmanID::BlueStickman)].punchRight = TextureID::PunchRight;
	data[static_cast<int>(StickmanID::BlueStickman)].punchLeft = TextureID::PunchLeft;
	data[static_cast<int>(StickmanID::BlueStickman)].getPunchRight = TextureID::GetPunchRight;
	data[static_cast<int>(StickmanID::BlueStickman)].getPunchLeft = TextureID::GetPunchLeft;


	data[static_cast<int>(StickmanID::RedStickman)].damageMultiplier = 1.0f;
	data[static_cast<int>(StickmanID::RedStickman)].speed = 400.f;
	data[static_cast<int>(StickmanID::RedStickman)].punchInterval = sf::seconds(0.5f);
	data[static_cast<int>(StickmanID::RedStickman)].maxAirTime = sf::seconds(0.5f);
	data[static_cast<int>(StickmanID::RedStickman)].texture = TextureID::RedStick;
	data[static_cast<int>(StickmanID::RedStickman)].runRight = TextureID::RedRunningRight;
	data[static_cast<int>(StickmanID::RedStickman)].runLeft = TextureID::RedRunningLeft;
	data[static_cast<int>(StickmanID::RedStickman)].jump = TextureID::RedJump;
	data[static_cast<int>(StickmanID::RedStickman)].punchRight = TextureID::RedPunchRight;
	data[static_cast<int>(StickmanID::RedStickman)].punchLeft = TextureID::RedPunchLeft;
	data[static_cast<int>(StickmanID::RedStickman)].getPunchRight = TextureID::RedGetPunchRight;
	data[static_cast<int>(StickmanID::RedStickman)].getPunchLeft = TextureID::RedGetPunchLeft;

	return data;
}


std::vector<IslandData> initializeIslandData()
{
	std::vector<IslandData> data(static_cast<int>(IslandID::TypeCount));
	data[static_cast<int>(IslandID::mainIsland)].texture = TextureID::mainIsland;
	data[static_cast<int>(IslandID::mainIsland)].textureRect = sf::IntRect(0, 0, 1045, 104);
	data[static_cast<int>(IslandID::mainIsland)].action = [](Stickman& a) {};

	return data;
}



//std::vector<PickupData> initializePickupData()
//{
//
//	//std::vector<PickupData> data(static_cast<int>(PickupID::TypeCount));
//
//	
//
//	//return data;
//	return 
//}

