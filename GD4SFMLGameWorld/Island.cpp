#include "Island.hpp"
#include "DataTable.hpp" //needed for access to datatable to gain access to Island details
#include "ResourceHolder.hpp" //needed for textures
#include "Utility.hpp" //needed for centreOrigin

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<IslandData> Table = initializeIslandData();
}

Island::Island(IslandID type, const TextureHolder& textures):Entity(),
	  mType(type)
	, mSprite(textures.get(Table[static_cast<int>(type)].texture), Table[static_cast<int>(type)].textureRect)
{
	centreOrigin(mSprite);
}

unsigned int Island::getCategory() const
{
	return static_cast<int>(CategoryID::mainIsland);
}

sf::FloatRect Island::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Island::apply(Stickman& player) const
{
	Table[static_cast<int>(mType)].action(player);
}

void Island::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}
