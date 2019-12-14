#include "DataTable.hpp"
#include "Stickman.hpp"
//#include "Pickup.hpp"
#include "StickmanID.hpp"
//#include "PickupID.hpp"



std::vector<StickmanData> initializeStickmanData()
{
	std::vector<StickmanData> data(static_cast<int>(StickmanID::TypeCount));

	data[static_cast<int>(StickmanID::BlueStickman)].damageMultiplier = 1;
	data[static_cast<int>(StickmanID::BlueStickman)].speed = 400.f;
	data[static_cast<int>(StickmanID::BlueStickman)].punchInterval = sf::seconds(1);
	data[static_cast<int>(StickmanID::BlueStickman)].maxAirTime = sf::seconds(0.5f);
	data[static_cast<int>(StickmanID::BlueStickman)].texture = TextureID::Stick;


	

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

