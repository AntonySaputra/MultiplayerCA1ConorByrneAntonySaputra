
#pragma once

#include "ResourceIdentifiers.hpp"
#include "TextureID.hpp"


#include "SFML/System/Time.hpp"
#include "SFML/Graphics/Color.hpp"

#include <vector>
#include <functional>

class Stickman;



struct StickmanData
{
	//damage multiplier is replacement for HP, 
	//the bigger it is the easier stickman go out of bounds
	int damageMultiplier;
	float speed;
	TextureID texture;
	sf::Time punchInterval;
	sf::Time maxAirTime;

};


struct PickupData
{
	std::function<void(Stickman&)> action;
	TextureID texture;
};

std::vector<StickmanData> initializeStickmanData();
std::vector<PickupData> initializePickupData();