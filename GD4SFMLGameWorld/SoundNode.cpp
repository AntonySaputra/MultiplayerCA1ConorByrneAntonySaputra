//Antony Saputra D00197615
//Conor Byrne D00185566

#include "SoundNode.hpp"

SoundNode::SoundNode(SoundPlayer& player)
	: SceneNode()
	, mSound(player)
{
}

void SoundNode::playSound(SoundEffectID sound, sf::Vector2f position)
{
	mSound.play(sound, position);
}

unsigned int SoundNode::getCategory() const
{
	return static_cast<int>(CategoryID::SoundEffect);
}
