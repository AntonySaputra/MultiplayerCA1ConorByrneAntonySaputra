#pragma once
#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"
#include "IslandID.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Stickman;

class Island : public Entity
{
public:
	Island(IslandID type, const TextureHolder& textures);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;

	void apply(Stickman& player) const;


protected:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	IslandID mType;
	sf::Sprite mSprite;
};