
#pragma once

#include "ResourceIdentifiers.hpp"
#include "TextureID.hpp"
#include "ActionID.hpp"
#include "CategoryID.hpp"


#include "SFML/System/Time.hpp"
#include "SFML/Graphics/Color.hpp"
#include <SFML/Window/Event.hpp>

#include <map>
#include <vector>
#include <functional>

class Stickman;

struct PlayerData
{
	std::map<sf::Keyboard::Key, ActionID> mKeyBinding;
	CategoryID categoryID;
};


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

std::vector<PlayerData> initializePlayerData();
std::vector<StickmanData> initializeStickmanData();
std::vector<PickupData> initializePickupData();